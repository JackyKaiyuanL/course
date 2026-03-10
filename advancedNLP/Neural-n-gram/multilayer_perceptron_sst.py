"""Multi-layer perceptron model for Assignment 1: Starter code.

You can change this code while keeping the function giving headers. You can add any functions that will help you. The given function headers are used for testing the code, so changing them will fail testing.


We adapt shape suffixes style when working with tensors.
See https://medium.com/@NoamShazeer/shape-suffixes-good-coding-style-f836e72e24fd.

Dimension key:

b: batch size
l: max sequence length
c: number of classes
v: vocabulary size

For example,

feature_b_l means a tensor of shape (b, l) == (batch_size, max_sequence_length).
length_1 means a tensor of shape (1) == (1,).
loss means a tensor of shape (). You can retrieve the loss value with loss.item().
"""

import argparse
import os
from collections import Counter
from pprint import pprint
from typing import Dict, List, Tuple

import pandas as pd
import torch
import torch.nn as nn
from torch.utils.data import DataLoader, Dataset
from tqdm import tqdm
from utils import DataPoint, DataType, accuracy, load_data, save_results


class Tokenizer:
    # The index of the padding embedding.
    # This is used to pad variable length sequences.
    TOK_PADDING_INDEX = 0
    STOP_WORDS = set(pd.read_csv("stopwords.txt", header=None)[0])

    def _pre_process_text(self, text: str) -> List[str]:
        t = text.lower()

        # keep these as standalone tokens
        for ch in ["!", "?", "@"]:
            t = t.replace(ch, f" {ch} ")

        # remove these (NOTE: keep apostrophes for don't, isn't, ...)
        for ch in ".,;:()[]{}\"`":
            t = t.replace(ch, " ")

        words = []
        for tok in t.split():
            if tok.isdigit():
                tok = "<num>"

            if tok in self.STOP_WORDS:
                continue

            words.append(tok)

        return words


    def __init__(self, data: List[DataPoint], max_vocab_size: int = None):
        corpus = " ".join([d.text for d in data])
        token_freq = Counter(self._pre_process_text(corpus))
        token_freq = token_freq.most_common(max_vocab_size)
        tokens = [t for t, _ in token_freq]
        # offset because padding index is 0
        self.token2id = {t: (i + 1) for i, t in enumerate(tokens)}
        self.token2id["<PAD>"] = Tokenizer.TOK_PADDING_INDEX
        self.id2token = {i: t for t, i in self.token2id.items()}

    def tokenize(self, text: str) -> List[int]:
        tokens = self._pre_process_text(text)
        ids: List[int] = []
        for tok in tokens:
            tok_id = self.token2id.get(tok, None)
            if tok_id is not None:
                ids.append(tok_id)
        return ids



def get_label_mappings(
    data: List[DataPoint],
) -> Tuple[Dict[str, int], Dict[int, str]]:
    """Reads the labels file and returns the mapping."""
    labels = list(set([d.label for d in data]))
    label2id = {label: index for index, label in enumerate(labels)}
    id2label = {index: label for index, label in enumerate(labels)}
    return label2id, id2label


class BOWDataset(Dataset):
    def __init__(
        self,
        data: List[DataPoint],
        tokenizer: Tokenizer,
        label2id: Dict[str, int],
        max_length: int = 100,
    ):
        super().__init__()
        self.data = data
        self.tokenizer = tokenizer
        self.label2id = label2id
        self.max_length = max_length

    def __len__(self):
        return len(self.data)

    def __getitem__(self, idx: int) -> Tuple[torch.Tensor, torch.Tensor, torch.Tensor]:
        dp: DataPoint = self.data[idx]

        tok_ids = self.tokenizer.tokenize(dp.text)
        length = min(len(tok_ids), self.max_length)

        tok_ids = tok_ids[: self.max_length]

        pad_id = self.tokenizer.TOK_PADDING_INDEX
        if len(tok_ids) < self.max_length:
            tok_ids = tok_ids + [pad_id] * (self.max_length - len(tok_ids))

        features_l = torch.tensor(tok_ids, dtype=torch.int64)
        length_t = torch.tensor(length, dtype=torch.int64)

        label_id = 0 if dp.label is None else self.label2id[dp.label]
        label_t = torch.tensor(label_id, dtype=torch.int64)

        return features_l, length_t, label_t




class MultilayerPerceptronModel(nn.Module):
    """Multi-layer perceptron model for classification."""

    def __init__(self, vocab_size: int, num_classes: int, padding_index: int):
        super().__init__()
        self.padding_index = padding_index

        self.emb_dim = 128
        self.maxvocab = 2000
        self.hidden_dim = 128
        self.dropout_p=0.1
        self.embedding = nn.Embedding(
            num_embeddings=vocab_size,
            embedding_dim=self.emb_dim,
            padding_idx=padding_index,
        )

        self.mlp = nn.Sequential(
            nn.Linear(self.emb_dim, self.maxvocab),

            nn.Linear(self.maxvocab, self.hidden_dim),
            nn.ReLU(),
            nn.Dropout(self.dropout_p),

            nn.Linear(self.hidden_dim, self.hidden_dim),
            nn.ReLU(),
            nn.Dropout(self.dropout_p),

            nn.Linear(self.hidden_dim, self.emb_dim),
            nn.Linear(self.emb_dim, num_classes)

        )


    def forward(self, input_features_b_l: torch.Tensor, input_length_b: torch.Tensor) -> torch.Tensor:
        emb_b_l_d = self.embedding(input_features_b_l)

        mask_b_l_1 = (input_features_b_l != self.padding_index).unsqueeze(-1)
        emb_b_l_d = emb_b_l_d * mask_b_l_1

        sum_b_d = emb_b_l_d.sum(dim=1)
        len_b_1 = input_length_b.clamp(min=1).unsqueeze(1).to(sum_b_d.dtype)
        mean_b_d = sum_b_d / len_b_1

        output_b_c = self.mlp(mean_b_d)
        return output_b_c



class Trainer:
    def __init__(self, model: nn.Module):
        self.model = model

    def predict(self, data: BOWDataset) -> List[int]:
        self.model.eval()
        all_predictions = []
        dataloader = DataLoader(data, batch_size=32, shuffle=False)

        device = next(self.model.parameters()).device
        with torch.no_grad():
            for inputs_b_l, lengths_b, _ in dataloader:
                inputs_b_l = inputs_b_l.to(device)
                lengths_b = lengths_b.to(device)

                logits_b_c = self.model(inputs_b_l, lengths_b)
                preds_b = torch.argmax(logits_b_c, dim=1)
                all_predictions.extend(preds_b.cpu().tolist())

        return all_predictions


    def evaluate(self, data: BOWDataset) -> float:
        self.model.eval()
        dataloader = DataLoader(data, batch_size=32, shuffle=False)

        device = next(self.model.parameters()).device
        correct = 0
        total = 0

        with torch.no_grad():
            for inputs_b_l, lengths_b, labels_b in dataloader:
                inputs_b_l = inputs_b_l.to(device)
                lengths_b = lengths_b.to(device)
                labels_b = labels_b.to(device)

                logits_b_c = self.model(inputs_b_l, lengths_b)
                preds_b = torch.argmax(logits_b_c, dim=1)

                correct += (preds_b == labels_b).sum().item()
                total += labels_b.size(0)

        return correct / max(total, 1)


    def train(
        self,
        training_data: BOWDataset,
        val_data: BOWDataset,
        optimizer: torch.optim.Optimizer,
        num_epochs: int,
    ) -> None:
        """Trains the MLP.

        Inputs:
            training_data: Suggested type for an individual training example is
                an (input, label) pair or (input, id, label) tuple.
                You can also use a dataloader.
            val_data: Validation data.
            optimizer: The optimization method.
            num_epochs: The number of training epochs.
        """
        torch.manual_seed(0)
        device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
        self.model.to(device)

        loss_fn = nn.CrossEntropyLoss()
        for epoch in range(num_epochs):
            self.model.train()
            total_loss = 0
            n_examples=0
            dataloader = DataLoader(training_data, batch_size=100, shuffle=True)
            for inputs_b_l, lengths_b, labels_b in tqdm(dataloader):
                inputs_b_l = inputs_b_l.to(device)
                lengths_b = lengths_b.to(device)
                labels_b = labels_b.to(device)

                optimizer.zero_grad()
                logits_b_c = self.model(inputs_b_l, lengths_b)

                loss = loss_fn(logits_b_c, labels_b)

                loss.backward()
                optimizer.step()

                total_loss += loss.item() * labels_b.size(0)
                n_examples += labels_b.size(0)
            per_dp_loss = 0

            self.model.eval()
            val_acc = self.evaluate(val_data)

            print(
                f"Epoch: {epoch + 1:<2} | Loss: {per_dp_loss:.2f} | Val accuracy: {100 * val_acc:.2f}%"
            )


if __name__ == "__main__":
    num_epochs = 50
    lr = 2e-3
    data_type = DataType('sst2')

    train_data, val_data, dev_data, test_data = load_data(data_type)

    tokenizer = Tokenizer(train_data, max_vocab_size=20000)
    label2id, id2label = get_label_mappings(train_data)
    print("Id to label mapping:")
    pprint(id2label)

    max_length = 100
    train_ds = BOWDataset(train_data, tokenizer, label2id, max_length)
    val_ds = BOWDataset(val_data, tokenizer, label2id, max_length)
    dev_ds = BOWDataset(dev_data, tokenizer, label2id, max_length)
    test_ds = BOWDataset(test_data, tokenizer, label2id, max_length)

    model = MultilayerPerceptronModel(
        vocab_size=len(tokenizer.token2id),
        num_classes=len(label2id),
        padding_index=Tokenizer.TOK_PADDING_INDEX,
    )

    trainer = Trainer(model)

    print("Training the model...")
    optimizer = torch.optim.Adam(model.parameters(), lr=lr)
    trainer.train(train_ds, val_ds, optimizer, num_epochs)

    # Evaluate on dev
    dev_acc = trainer.evaluate(dev_ds)
    print(f"Development accuracy: {100 * dev_acc:.2f}%")

    # Predict on test
    test_preds = trainer.predict(test_ds)
    test_preds = [id2label[pred] for pred in test_preds]
    save_results(
        test_data,
        test_preds,
        os.path.join("results", f"mlp_sst2_test_predictions.csv"),
    )

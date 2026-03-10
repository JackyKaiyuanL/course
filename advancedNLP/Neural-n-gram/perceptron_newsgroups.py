"""Perceptron model model for Assignment 1: Starter code.

You can change this code while keeping the function giving headers. You can add any functions that will help you. The given function headers are used for testing the code, so changing them will fail testing.
"""

import argparse
import json
import os
from collections import defaultdict
from dataclasses import dataclass
from typing import Dict, List, Set

from features import make_featurize
from tqdm import tqdm
from utils import DataPoint, DataType, accuracy, load_data, save_results


@dataclass(frozen=True)
class DataPointWithFeatures(DataPoint):
    features: Dict[str, float]


def featurize_data(
    data: List[DataPoint], feature_types: Set[str]
) -> List[DataPointWithFeatures]:
    """Add features to each datapoint based on feature types"""
    featurize = make_featurize(feature_types)  # build once

    data_with_features: List[DataPointWithFeatures] = []
    for dp in tqdm(data, desc="Featurizing data"):
        features = featurize(dp.text)
        data_with_features.append(
            DataPointWithFeatures(id=dp.id, text=dp.text, label=dp.label, features=features)
        )

    return data_with_features


class PerceptronModel:
    """Perceptron model for classification."""

    def __init__(self):
        self.weights: Dict[str, float] = defaultdict(float)
        self.labels: Set[str] = set()

    def _get_weight_key(self, feature: str, label: str) -> str:
        """An internal hash function to build keys of self.weights (needed for tests)"""
        return feature + "#" + str(label)

    def score(self, datapoint: DataPointWithFeatures, label: str) -> float:
        """Compute the score of a class given the input.

        Inputs:
            datapoint (Datapoint): a single datapoint with features populated
            label (str): label

        Returns:
            The output score.
        """
        total = 0.0
        for feature, value in datapoint.features.items():
            key = self._get_weight_key(feature, label)
            total += self.weights[key] * value
        return total

    def predict(self, datapoint: DataPointWithFeatures) -> str:
        """Predicts a label for an input.

        Inputs:
            datapoint: Input data point.

        Returns:
            The predicted class.
        """
        # TODO: Implement this! Expected # of lines: <5

        return max(self.labels, key=lambda label: self.score(datapoint, label))


    def update_parameters(
        self, datapoint: DataPointWithFeatures, prediction: str, lr: float
    ) -> None:
        """Update the model weights of the model using the perceptron update rule.

        Inputs:
            datapoint: The input example, including its label.
            prediction: The predicted label.
            lr: Learning rate.
        """
        # TODO: Implement this! Expected # of lines: <10
        gold = datapoint.label
        if prediction == gold:
            return

        for feature, value in datapoint.features.items():
            self.weights[self._get_weight_key(feature, gold)] += lr * value
            self.weights[self._get_weight_key(feature, prediction)] -= lr * value


    def train(
        self,
        training_data: List[DataPointWithFeatures],
        val_data: List[DataPointWithFeatures],
        num_epochs: int,
        lr: float,
    ) -> None:
        """Perceptron model training. Updates self.weights and self.labels
        We greedily learn about new labels.

        Inputs:
            training_data: Suggested type is (list of tuple), where each item can be
                a training example represented as an (input, label) pair or (input, id, label) tuple.
            val_data: Validation data.
            num_epochs: Number of training epochs.
            lr: Learning rate.
        """
        for dp in training_data:
            self.labels.add(dp.label)

        for epoch in range(num_epochs):
            for dp in training_data:
                prediction = self.predict(dp)
                self.update_parameters(dp, prediction, lr)
            # for dp in val_data:
            #     prediction = self.predict(dp)
            #     self.update_parameters(dp, prediction, lr)
            print(f"Epoch {epoch} done, val accuarcy {model.evaluate(val_data)}")



    def save_weights(self, path: str) -> None:
        with open(path, "w") as f:
            f.write(json.dumps(self.weights, indent=2, sort_keys=True))
        print(f"Model weights saved to {path}")

    def evaluate(
        self,
        data: List[DataPointWithFeatures],
        save_path: str = None,
    ) -> float:
        """Evaluates the model on the given data.

        Inputs:
            data (list of Datapoint): The data to evaluate on.
            save_path: The path to save the predictions.

        Returns:
            accuracy (float): The accuracy of the model on the data.
        """
        # TODO: Implement this!
        preds: List[str] = []
        targets: List[str] = []
        for dp in data:
            prediction = self.predict(dp)
            preds.append(prediction)
            targets.append(dp.label)
        if save_path is not None:
          save_results(data, preds, save_path)

        return accuracy(preds, targets)


if __name__ == "__main__":
    # ----------------------------
    # Hard-coded settings for 20NG
    # ----------------------------
    data_type = DataType("newsgroups")
    feature_types: Set[str] = set("bow+len".split("+"))
    num_epochs: int = 10
    lr: float = 0.1

    train_data, val_data, dev_data, test_data = load_data(data_type)

    train_data = featurize_data(train_data, feature_types)
    val_data = featurize_data(val_data, feature_types)
    dev_data = featurize_data(dev_data, feature_types)
    test_data = featurize_data(test_data, feature_types)

    model = PerceptronModel()
    print("Training the model...")
    model.train(train_data, val_data, num_epochs, lr)

    os.makedirs("results", exist_ok=True)

    # Dev evaluation (prints accuracy and saves predictions)
    dev_acc = model.evaluate(dev_data)
    print(f"Development accuracy: {100 * dev_acc:.2f}%")

    # Test predictions (label may be None; still saves CSV)
    _ = model.evaluate(
        test_data,
        save_path=os.path.join(
            "results",
            f"perceptron_newsgroups_test_predictions.csv",
        ),
    )

    # model.save_weights(
    #     os.path.join(
    #         "results",
    #         f"perceptron_newsgroups_{'+'.join(sorted(feature_types))}_model.json",
    #     )
    # )

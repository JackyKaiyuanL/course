"""
Neural network utilities for Transformer implementation.
Contains basic building blocks: softmax, cross-entropy, gradient clipping, token accuracy, perplexity.
"""
import torch
from torch import Tensor


def softmax(x: Tensor, dim: int = -1) -> Tensor:
    """
    Compute softmax along the specified dimension.
    
    Args:
        x: Input tensor of any shape
        dim: Dimension along which to compute softmax (default: -1)
    
    Returns:
        Tensor of same shape as input with softmax applied along dim
    """
    # TODO: Implement numerically stable softmax. You can re-use the same one 
    # used in part 2. But for this problem, you need to implement a numerically stable version to pass harder tests.
    x_max = torch.max(x, dim=dim, keepdim=True).values
    x_shifted = x - x_max
    exp_x = torch.exp(x_shifted)
    sum_exp = torch.sum(exp_x, dim=dim, keepdim=True)
    return exp_x / sum_exp
    # raise NotImplementedError("Implement softmax")


def cross_entropy(logits: Tensor, targets: Tensor) -> Tensor:
    """
    Compute cross-entropy loss.
    
    Args:
        logits: Unnormalized log probabilities of shape (N, C) where N is batch size
                and C is number of classes
        targets: Ground truth class indices of shape (N,)
    
    Returns:
        Scalar tensor containing the mean cross-entropy loss
    """
    # TODO: Implement cross-entropy loss
    if logits.ndim != 2:
        raise ValueError(f"logits must be 2D (N, C), got shape {tuple(logits.shape)}")
    if targets.ndim != 1:
        raise ValueError(f"targets must be 1D (N,), got shape {tuple(targets.shape)}")
    if logits.shape[0] != targets.shape[0]:
        raise ValueError("Batch size mismatch between logits and targets")

    # Numerically stable log-softmax:
    # log_softmax(x) = x - logsumexp(x)
    x_max = torch.max(logits, dim=1, keepdim=True).values
    x_shifted = logits - x_max
    logsumexp = torch.log(torch.sum(torch.exp(x_shifted), dim=1, keepdim=True))
    log_probs = x_shifted - logsumexp  # (N, C)

    # Pick the log-prob of the correct class for each row
    nll = -log_probs.gather(dim=1, index=targets.view(-1, 1)).squeeze(1)  # (N,)
    return nll.mean()
    # raise NotImplementedError("Implement cross_entropy")


def gradient_clipping(parameters, max_norm: float) -> Tensor:
    """
    Clip gradients of parameters by global norm.
    
    Args:
        parameters: Iterable of parameters with gradients
        max_norm: Maximum allowed gradient norm
    
    Returns:
        The total norm of the gradients before clipping
    """
    # TODO: Implement gradient clipping
    if max_norm <= 0:
        raise ValueError("max_norm must be > 0")

    params = [p for p in parameters if p is not None and p.grad is not None]
    if len(params) == 0:
        # No gradients to clip
        return torch.tensor(0.0)

    # Compute global grad norm: sqrt(sum ||g_i||^2)
    device = params[0].grad.device
    total_sq = torch.zeros((), device=device)
    for p in params:
        g = p.grad.detach()
        total_sq += torch.sum(g * g)
    total_norm = torch.sqrt(total_sq)

    # Clip if needed: g <- g * (max_norm / (total_norm + eps))
    eps = 1e-6
    clip_coef = max_norm / (total_norm + eps)
    if clip_coef < 1.0:
        for p in params:
            p.grad.detach().mul_(clip_coef)

    return total_norm
    # raise NotImplementedError("Implement gradient_clipping")


def token_accuracy(logits: Tensor, targets: Tensor, ignore_index: int = -100) -> Tensor:
    """
    Compute token-level accuracy for language modeling.
    
    Computes the fraction of tokens where the predicted token (argmax of logits)
    matches the target token, ignoring positions where target equals ignore_index.
    
    Args:
        logits: Predicted logits of shape (N, C) where N is the number of tokens
                and C is the vocabulary size
        targets: Ground truth token indices of shape (N,)
        ignore_index: Target value to ignore when computing accuracy (default: -100)
    
    Returns:
        Scalar tensor containing the accuracy (between 0 and 1)
    
    Example:
        >>> logits = torch.tensor([[2.0, 1.0, 0.5], [0.1, 3.0, 0.2], [1.0, 0.5, 2.5]])
        >>> targets = torch.tensor([0, 1, 2])
        >>> token_accuracy(logits, targets)
        tensor(1.)  # All predictions correct: argmax gives [0, 1, 2]
        
        >>> logits = torch.tensor([[2.0, 1.0], [0.1, 3.0], [1.0, 0.5]])
        >>> targets = torch.tensor([1, 1, 0])
        >>> token_accuracy(logits, targets)
        tensor(0.6667)  # 2 out of 3 correct
    """
    # TODO: Implement token accuracy
    if logits.ndim != 2:
        raise ValueError(f"logits must be 2D (N, C), got shape {tuple(logits.shape)}")
    if targets.ndim != 1:
        raise ValueError(f"targets must be 1D (N,), got shape {tuple(targets.shape)}")
    if logits.shape[0] != targets.shape[0]:
        raise ValueError("N mismatch between logits and targets")

    preds = torch.argmax(logits, dim=1)  # (N,)
    mask = targets != ignore_index

    denom = mask.sum()
    if denom.item() == 0:
        # No valid tokens; define accuracy as 0.0 to avoid NaN
        return torch.tensor(0.0, device=logits.device)

    correct = (preds == targets) & mask
    return correct.float().sum() / denom
    # raise NotImplementedError("Implement token_accuracy")


def perplexity(logits: Tensor, targets: Tensor, ignore_index: int = -100) -> Tensor:
    """
    Compute perplexity for language modeling.
    
    Perplexity is defined as exp(cross_entropy_loss). It measures how well the
    probability distribution predicted by the model matches the actual distribution
    of the tokens. Lower perplexity indicates better prediction.
    
    Args:
        logits: Predicted logits of shape (N, C) where N is the number of tokens
                and C is the vocabulary size
        targets: Ground truth token indices of shape (N,)
        ignore_index: Target value to ignore when computing perplexity (default: -100)
    
    Returns:
        Scalar tensor containing the perplexity (always >= 1)
    
    Example:
        >>> # Perfect predictions (one-hot logits matching targets)
        >>> logits = torch.tensor([[10.0, 0.0, 0.0], [0.0, 10.0, 0.0], [0.0, 0.0, 10.0]])
        >>> targets = torch.tensor([0, 1, 2])
        >>> perplexity(logits, targets)
        tensor(1.0001)  # Close to 1 (perfect)
        
        >>> # Uniform predictions (high uncertainty)
        >>> logits = torch.tensor([[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]])
        >>> targets = torch.tensor([0, 1, 2])
        >>> perplexity(logits, targets)
        tensor(3.)  # Equal to vocab_size (worst case for uniform)
    """
    # TODO: Implement perplexity
    if logits.ndim != 2:
        raise ValueError(f"logits must be 2D (N, C), got shape {tuple(logits.shape)}")
    if targets.ndim != 1:
        raise ValueError(f"targets must be 1D (N,), got shape {tuple(targets.shape)}")
    if logits.shape[0] != targets.shape[0]:
        raise ValueError("N mismatch between logits and targets")

    mask = targets != ignore_index
    if mask.sum().item() == 0:
        return torch.tensor(float("inf"), device=logits.device)

    # Keep only valid rows (so targets are all in [0, C-1])
    logits_v = logits[mask]          # (Nv, C)
    targets_v = targets[mask]        # (Nv,)

    # stable log-softmax on valid rows
    x_max = torch.max(logits_v, dim=1, keepdim=True).values
    x_shifted = logits_v - x_max
    logsumexp = torch.log(torch.sum(torch.exp(x_shifted), dim=1, keepdim=True))
    log_probs = x_shifted - logsumexp  # (Nv, C)

    nll = -log_probs.gather(dim=1, index=targets_v.view(-1, 1)).squeeze(1)  # (Nv,)
    return torch.exp(nll.mean())
    # raise NotImplementedError("Implement perplexity")

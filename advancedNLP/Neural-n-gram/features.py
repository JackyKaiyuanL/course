from collections import ChainMap
from typing import Callable, Dict, Set

import pandas as pd


class FeatureMap:
    name: str

    @classmethod
    def featurize(self, text: str) -> Dict[str, float]:
        pass

    @classmethod
    def prefix_with_name(self, d: Dict) -> Dict[str, float]:
        """just a handy shared util function"""
        return {f"{self.name}/{k}": v for k, v in d.items()}


# class BagOfWords(FeatureMap):
#     name = "bow"
#     STOP_WORDS = set(pd.read_csv("stopwords.txt", header=None)[0])

#     @classmethod
#     def featurize(self, text: str) -> Dict[str, float]:
#         # TODO: implement this! Expected # of lines: <5
#         words = [w for w in text.lower().split() if w not in self.STOP_WORDS]
#         return self.prefix_with_name({w: 1.0 for w in words})

# class BagOfWords(FeatureMap):
#     name = "bow"
#     STOP_WORDS = set(pd.read_csv("stopwords.txt", header=None)[0])

#     @classmethod
#     def featurize(self, text: str) -> Dict[str, float]:
#         feats = {}
#         t = text.lower()

#         # keep these as tokens
#         for ch in ["!", "?", "@"]:
#             t = t.replace(ch, f" {ch} ")

#         # remove these
#         for ch in ".,;:()[]{}\"`":
#             t = t.replace(ch, " ")

#         for tok in t.split():
#             if any(c.isdigit() for c in tok):
#                 tok = "<num>"

#             if tok in self.STOP_WORDS:
#                 continue

#             feats[tok] = 1.0

#         return self.prefix_with_name(feats)
@classmethod
def featurize(self, text: str) -> Dict[str, float]:
    feats = {}
    t = text.lower()

    for ch in ["!", "?", "@"]:
        t = t.replace(ch, f" {ch} ")

    for ch in ".,;:()[]{}\"`":
        t = t.replace(ch, " ")

    for tok in t.split():
        # less aggressive number normalization
        if tok.isdigit():
            tok = "<num>"

        if tok in self.STOP_WORDS:
            continue

        feats[tok] = 1.0

    return self.prefix_with_name(feats)


class SentenceLength(FeatureMap):
    name = "len"

    @classmethod
    def featurize(self, text: str) -> Dict[str, float]:
        """an example of custom feature that rewards long sentences"""
        if len(text.split()) < 10:
            k = "short"
            v = 1.0
        else:
            k = "long"
            v = 5.0
        ret = {k: v}
        return self.prefix_with_name(ret)
class NegationScope(FeatureMap):
    name = "neg"
    NEGATORS = {"not", "no", "never", "n't"}

    @classmethod
    def featurize(self, text: str) -> Dict[str, float]:
        toks = text.lower().split()
        ret = {}

        neg_on = False
        window = 0
        for w in toks:
            if w in self.NEGATORS or w.endswith("n't"):
                ret["has_negation"] = 1.0
                neg_on = True
                window = 3
                continue
            if neg_on and window > 0:
                ret[f"neg_{w}"] = ret.get(f"neg_{w}", 0.0) + 1.0
                window -= 1
                if window == 0:
                    neg_on = False

        return self.prefix_with_name(ret)
class Emphasis(FeatureMap):
    name = "emp"

    @classmethod
    def featurize(self, text: str) -> Dict[str, float]:
        t = text
        words = t.split()
        ret = {}

        if "!" in t: ret["has_exclaim"] = 1.0
        if "?" in t: ret["has_question"] = 1.0
        if "!!" in t: ret["multi_exclaim"] = 1.0
        if "??" in t: ret["multi_question"] = 1.0

        caps = sum(1 for w in words if w.isalpha() and w.isupper())
        if caps > 0:
            ret["has_allcaps_word"] = 1.0
            ret["allcaps_count"] = float(caps)

        # simple “intensity” feature: longer punctuation runs
        ret["exclaim_count"] = float(t.count("!"))
        ret["question_count"] = float(t.count("?"))

        return self.prefix_with_name(ret)
class NewsStyle(FeatureMap):
    name = "style"

    @classmethod
    def featurize(self, text: str) -> Dict[str, float]:
        t = text
        lines = t.splitlines()
        ret = {}

        # email-ish markers
        if "@" in t: ret["has_at"] = 1.0
        if "http://" in t.lower() or "https://" in t.lower(): ret["has_url"] = 1.0

        # quoting / replies
        quote_lines = sum(1 for ln in lines if ln.lstrip().startswith(">"))
        if quote_lines > 0:
            ret["has_quote_lines"] = 1.0
            ret["quote_line_count"] = float(quote_lines)

        # header-like fields
        header_keys = ("subject:", "organization:", "lines:", "nntp-posting-host:", "distribution:")
        header_hits = sum(1 for hk in header_keys if hk in t.lower())
        if header_hits > 0:
            ret["has_headers"] = 1.0
            ret["header_hit_count"] = float(header_hits)

        return self.prefix_with_name(ret)
class TechPatterns(FeatureMap):
    name = "tech"

    @classmethod
    def featurize(self, text: str) -> Dict[str, float]:
        t = text
        toks = t.split()
        ret = {}

        # file/path markers
        if "/" in t or "\\" in t: ret["has_path_sep"] = 1.0
        if ":" in t: ret["has_colon"] = 1.0

        # file extensions common in comp.* / graphics
        exts = (".exe", ".dll", ".sys", ".bat", ".com", ".gif", ".jpg", ".jpeg", ".png", ".c", ".h", ".cpp")
        if any(e in t.lower() for e in exts):
            ret["has_file_ext"] = 1.0

        # digit-heavy tokens (versions, model numbers, IPs)
        num_tokens = sum(1 for w in toks if any(ch.isdigit() for ch in w))
        if num_tokens > 0:
            ret["has_number_token"] = 1.0
            ret["number_token_count"] = float(num_tokens)

        # math/crypto-ish symbols often appear in sci.crypt etc.
        sym_count = sum(t.count(s) for s in ["=", "+", "*", "^"])
        if sym_count > 0:
            ret["math_symbol_count"] = float(sym_count)

        return self.prefix_with_name(ret)


FEATURE_CLASSES_MAP = {c.name: c for c in [BagOfWords, SentenceLength,NegationScope, Emphasis, NewsStyle, TechPatterns]}


def make_featurize(
    feature_types: Set[str],
) -> Callable[[str], Dict[str, float]]:
    featurize_fns = [FEATURE_CLASSES_MAP[n].featurize for n in feature_types]

    def _featurize(text: str):
        f = ChainMap(*[fn(text) for fn in featurize_fns])
        return dict(f)

    return _featurize


__all__ = ["make_featurize"]

if __name__ == "__main__":
    text = "I love this movie"
    print(text)
    print(BagOfWords.featurize(text))
    featurize = make_featurize({"bow", "len"})
    print(featurize(text))

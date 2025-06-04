import os
import random
from string import ascii_lowercase
import sys

ARTICLES = ["a", "an", "A", "An"]

PUNCTUATION = [".", "!", "?", ";", ":"]

EDGE_CASES = (
    [
        "",  # empty string
        "word",  # one word
        "a ",  # article with a space
        "an ",  # article with a space
        "a.b",  # dot inside of a word
        "A! test",  # exclamaition after an article
        "AN unusual CASE",  # article upper case
    ]
    + ARTICLES
    + PUNCTUATION
)


def generate_random_sentence(min_words=3, max_words=15):
    words = []
    word_count = random.randint(min_words, max_words)

    if random.random() < 0.05:
        return random.choice(EDGE_CASES)

    if random.random() < 0.3:
        words.append(random.choice(ARTICLES))

    for _ in range(word_count):
        word = "".join(
            random.choice(ascii_lowercase)
            for _ in range(random.randint(2, 10))
        )
        if random.random() < 0.2:
            word = word.capitalize()
        words.append(word)

    sentence = " ".join(words)

    if random.random() < 0.7:
        sentence += random.choice(PUNCTUATION)

    return sentence


def process_line(line, line_num):
    words = line.split()
    if words:
        first_word = words[0].lower()
        if first_word in ARTICLES:
            words = words[1:]

    line = " ".join(words)
    line = line.upper() if line_num % 2 == 1 else line.lower()

    if (
        len(line) == 0
        or len(line) > 0
        and line[-1] not in {".", "!", "?", ";", ":"}
    ):
        line += "."

    return line


def generate_test_cases(n: int = 10):
    def write_input_expected(
        i: int, input_lines: list[str], expected_lines: list[str]
    ):
        with open(f"tests/test_{i}_input.txt", "w", encoding="utf-8") as f:
            f.write("\n".join(input_lines))

        with open(f"tests/test_{i}_expected.txt", "w", encoding="utf-8") as f:
            f.write("\n".join(expected_lines))

    os.makedirs("tests", exist_ok=True)

    for i in range(1, n + 2):
        input_lines, expected_lines = [], []
        if i == 1:
            input_lines = EDGE_CASES
            expected_lines = [
                process_line(line, index)
                for index, line in enumerate(EDGE_CASES, 1)
            ]
        else:
            num_lines = random.randint(1, 1000)

            for line_num in range(1, num_lines + 1):
                line = generate_random_sentence()
                input_lines.append(line)
                expected_lines.append(process_line(line, line_num))

        write_input_expected(i, input_lines, expected_lines)

    print(f"Generated {n} test cases in tests/ directory")


def print_help():
    print("python gen.py [num_tests]")
    sys.exit()


if __name__ == "__main__":
    num_tests = 10
    if len(sys.argv) > 2:
        print_help()
    elif len(sys.argv) == 2:
        try:
            num_tests = int(sys.argv[1])
        except Exception:
            print_help()

    generate_test_cases(num_tests)

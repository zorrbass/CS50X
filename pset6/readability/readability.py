import cs50
import re                                   # import of regular expressions

words = cs50.get_string("Text: ")           # read text into words string

Lc_pattern = re.compile(r'[a-zA-Z]')        # define pattern to search for letters
Lc_matches = Lc_pattern.finditer(words)     # find matches
Lc = 0.0
for match in Lc_matches:                    # count matches
    Lc += 1
    
Wc_pattern = re.compile(r"[ ]")             # define pattern to search for words
Wc_matches = Wc_pattern.finditer(words)
Wc = 1.0                                    # last word doesn't have a space after
for match in Wc_matches:
    Wc += 1

Sc_pattern = re.compile(r'[.!?]')           # define pattern to search for sentences
Sc_matches = Sc_pattern.finditer(words)
Sc = 0.0
for match in Sc_matches:
    Sc += 1
    
I = 0.0588 * ((Lc * 100) / (Wc)) - 0.296 * ((Sc * 100) / (Wc)) - 15.8   # Coleman-Liau index

# print results
if I < 1:
    print("Before Grade 1")
elif I > 16:
    print("Grade 16+")
else:
    print(f"Grade {round(I)}")
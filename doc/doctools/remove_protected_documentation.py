import sys

# Read the first argument which is the input file
filename_input = sys.argv[1]

# Read the input file into a list
with open(filename_input, "r") as f:
    file_lines = f.readlines()

# Make a list of strings that indicate that the function is protected
protected_indicator_strings = []
protected_indicator_strings.append("<code> [protected, inherited]</code>")
protected_indicator_strings.append("<code> [static, protected, inherited]</code>")
protected_indicator_strings.append("<code> [inline, protected, inherited]</code>")
protected_indicator_strings.append("<code> [inline, protected]</code>")

i = 0
while i < len(file_lines):
    line = file_lines[i].strip()
    # Search for the anchor links
    if len(line) >= 18 and line[0:17] == "<a class=\"anchor\"":
        closing_divs_found = False
        protected_found = False
        # Go through the successive lines
        j = i + 1
        while j < len(file_lines) - 1 and not closing_divs_found:
            # Test whether the word "protected" is there in the right format
            if [s in file_lines[j + 1] for s in protected_indicator_strings].count(True) >= 1:
                protected_found = True;

            # Look for two closing divs in this line and in the next line
            if file_lines[j].strip() == "</div>" and file_lines[j + 1].strip() == "</div>":
                closing_divs_found = True

            # Increment the counting parameter
            j += 1

        # If no protected was found, write the area
        if not protected_found:
            for l in range(i, j + 1):
                sys.stdout.write(file_lines[l])
        # Increment the line after the considered block
        i = j + 1

    # Otherwise simply write the file
    else:
        sys.stdout.write(file_lines[i])
        i += 1
        

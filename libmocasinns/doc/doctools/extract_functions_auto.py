import sys

# Read the first argument which is the input file
filename_input = sys.argv[1]

# Read the input file into a list
with open(filename_input, "r") as f:
    file_lines = f.readlines()

for i in range(0, len(file_lines)):
    line = file_lines[i].strip()
    if "/*! \\fn" in line or "/*! \\file" in line:
        # Find the line with the closing structure
        for j in range(i, len(file_lines)):
            if "*/" in file_lines[j]:
                break
        # Write to the output
        if line == "/*! \\fn":
            function_name = file_lines[j + 1].strip()
            sys.stdout.write(file_lines[i] + function_name + "\n")
            for l in range(i + 1,j + 1):
                sys.stdout.write(file_lines[l])
        elif line ==  "/*! \\fn AUTO":
            function_name = file_lines[j + 1].strip()
            sys.stdout.write(file_lines[i][:-5] + function_name + "\n")
            for l in range(i + 1,j + 1):
                sys.stdout.write(file_lines[l])
        elif line == "/*! \\fn AUTO_TEMPLATE_1":
            function_name = file_lines[j + 2].strip()
            sys.stdout.write(file_lines[i][:-16] + function_name + "\n")
            for l in range(i + 1,j + 1):
                sys.stdout.write(file_lines[l])
        elif line == "/*! \\fn AUTO_TEMPLATE_2":
            function_template_name = file_lines[j + 2].strip()
            function_name = file_lines[j + 3].strip()
            sys.stdout.write(file_lines[i][:-16] + function_template_name + " " + function_name + "\n")
            for l in range(i + 1,j + 1):
                sys.stdout.write(file_lines[l])
        else:
            for l in range(i,j+1):
                sys.stdout.write(file_lines[l])
        # Write a newline for convenience
        sys.stdout.write("\n")
    



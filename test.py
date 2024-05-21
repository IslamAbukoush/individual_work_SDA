from flask import Flask, request, render_template
import subprocess
import json

app = Flask(__name__)

# Function to replace special characters in the input string
def replace_special_characters(input_string):
    replacements = {
        "<": "_lt_",
        ">": "_gt_",
        ":": "_colon_",
        "\"": "_quote_",
        "/": "_slash_",
        "\\": "_backslash_",
        "|": "_pipe_",
        "?": "_questionmark_",
        "*": "_asterisk_",
        "\n": "_newline_",
        "&": "^&",
    }
    for char, replacement in replacements.items():
        input_string = input_string.replace(char, replacement)
    return input_string

# Function to process the input string in chunks
def process_string(input_string):
    # Replace special characters in the input string
    escaped_input_string = replace_special_characters(input_string)

    # Call the C program with the escaped code snippet as an argument
    result = subprocess.run(["./main", escaped_input_string], capture_output=True, text=True)

    # Parse the JSON output and extract the content string
    try:
        output_json = json.loads(result.stdout)
        print(output_json)
        content = output_json["choices"][0]["message"]["content"]
        # Decode the newline character back
        content = content.replace("_newline_", "\n")
        return content
    except (json.JSONDecodeError, IndexError, KeyError) as e:
        return "Error parsing JSON output:", e

@app.route('/', methods=['GET', 'POST'])
def index():
    if request.method == 'POST':
        code_snippet = request.form['code_snippet']
        result = process_string(code_snippet)
        result = [line for line in result.split('\n') if line.strip()]
        return render_template('result.html', result=result)
    return render_template('index.html')

if __name__ == '__main__':
    app.run(debug=True)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to find a substring in a string
int findSubstring(const char *find, const char *content) {
    const char *ptr = strstr(content, find);
    return ptr != NULL ? ptr - content : -1;
}

int main(int argc, char *argv[]) {
    // Ensure correct usage
    if (argc != 2) {
        printf("Usage: %s \"your_text_here\"\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Command template for making API request
    const char *command_template = "curl -sS -X POST https://api.openai.com/v1/chat/completions \
                                    -H \"Content-Type: application/json\" \
                                    -H \"Authorization: Bearer sk-proj-efe4Cvs7dAjKYEVi3HCrT3BlbkFJW0xuVe7U2jy4M2mnp2JD\" \
                                    -d \"{\\\"model\\\": \\\"gpt-3.5-turbo\\\", \\\"messages\\\": [{\\\"role\\\": \\\"system\\\", \\\"content\\\": \\\"You are a time/space complexity calculator and your job is to calculate the time/space complexity for any source code that is given to you, now the format, you will give back two lines only, you are not allowed to do more or less, first line will contain time complexity, will go like this 'Time Complexity:' and followed with the correct time complexity along side some reasons for why this is the time complexity, and on a new line 'Space Complexity:' in the same format.\\\"},{\\\"role\\\": \\\"user\\\", \\\"content\\\": \\\"%s\\\"}]}\"";

    // Create the command string with the provided text
    char command[16384];
    snprintf(command, sizeof(command), command_template, argv[1]);

    // Open a pipe to read the command output
    FILE *pipe = popen(command, "r");
    if (!pipe) {
        perror("Error executing command");
        return EXIT_FAILURE;
    }

    // Buffer to store the command output
    char buffer[16384];
    char *json_response = NULL;

    // Read the command output
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        // Append buffer to json_response
        if (json_response == NULL) {
            json_response = strdup(buffer);
        } else {
            char *temp = strdup(json_response);
            free(json_response);
            int new_length = strlen(temp) + strlen(buffer) + 1;
            json_response = (char *)malloc(new_length);
            snprintf(json_response, new_length, "%s%s", temp, buffer);
            free(temp);
        }
    }

    // Close the pipe
    pclose(pipe);

    printf("%s", json_response);

    // Free allocated memory
    free(json_response);

    return EXIT_SUCCESS;
}

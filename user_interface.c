/*
 * Rishi Erigineni
 * rerigine
 * 118908021
 */

#include "document.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

/* Function declarations for main function */
int add_para_after(Document *doc, char *cmd);
int add_ln_after(Document *doc, char *cmd);
int print_doc(Document *doc, char *cmd);
int append_ln(Document *doc, char *cmd);
int remove_ln(Document *doc, char *cmd);
int load_fl(Document *doc, char *cmd);
char *debrief(char *cmd, char *result);
int cmd_run(Document *doc, char *command);
int replace_txt(Document *doc, char *cmd);
int highlight_txt(Document *doc, char *cmd);
int remove_txt(Document *doc, char *cmd);
int save_doc(Document *doc, char *cmd);
int reset_doc(Document *doc, char *cmd);

/* Main function */
int main(int argc, char *argv[]) {
    FILE *file;
    Document doc;
    const char *doc_identifier = "main_document";
    char user_input[MAX_STR_SIZE + 1];
    char i, j[MAX_STR_SIZE + 1];
    int num_values = 0;

    /* Initialize the document */
    if (init_document(&doc, doc_identifier) == FAILURE) {
        printf("Initialization failed\n");
    } else {
        /* Check command-line arguments */
        if (argc == 1) {
            file = stdin;

            /* Read user input from the command line */
            printf("> ");
            fgets(user_input, MAX_STR_SIZE + 1, stdin);

            while (strcmp(user_input, "quit\n") && strcmp(user_input, "exit\n")) {
                sscanf(user_input, " %c%s", &i, j);
                if (strcmp(user_input, "\n") != 0 && i != '#') {
                    cmd_run(&doc, user_input);
                }
                printf("> ");
                fgets(user_input, MAX_STR_SIZE + 1, stdin);
            }
        } else if (argc == 2) {
            /* Open the specified file */
            if ((file = fopen(argv[1], "r")) == NULL) {
                fprintf(stderr, "%s cannot be opened.\n", argv[1]);
                exit(EX_OSERR);
            } else {
                /* Read commands from the file */
                while (fgets(user_input, MAX_STR_SIZE + 1, file)) {
                    if (strcmp(user_input, "quit\n") && strcmp(user_input, "exit\n")) {
                        num_values = sscanf(user_input, " %c%s", &i, j);
                        if (num_values > 0 && i != '#') {
                            cmd_run(&doc, user_input);
                        }
                    }
                }
            }
        } else {
            fprintf(stderr, "Usage: user_interface\n");
            fprintf(stderr, "Usage: user_interface <filename>\n");
            exit(EX_USAGE);
        }
    }
    
    /* Close the file used */
    fclose(file);

    /* Exit the program */
    exit(EXIT_SUCCESS);
}

/* Function to run a command based on user input */
int cmd_run(Document *doc, char *command) {
    int bin = FAILURE;

    if (strstr(command, "add_paragraph_after") != NULL) {
        if (!add_para_after(doc, command)) {
            bin = SUCCESS;
        }
    } else if (strstr(command, "add_line_after") != NULL) {
        if (!add_ln_after(doc, command)) {
            bin = SUCCESS;
        }
    } else if (strstr(command, "print_document") != NULL) {
        if (!print_doc(doc, command)) {
            bin = SUCCESS;
        }
    } else if (strstr(command, "append_line") != NULL) {
        if (!append_ln(doc, command)) {
            bin = SUCCESS;
        }
    } else if (strstr(command, "remove_line") != NULL) {
        if (!remove_ln(doc, command)) {
            bin = SUCCESS;
        }
    } else if (strstr(command, "load_file") != NULL) {
        if (!load_fl(doc, command)) {
            bin = SUCCESS;
        }
    } else if (strstr(command, "replace_text") != NULL) {
        if (!replace_txt(doc, command)) {
            bin = SUCCESS;
        }
    } else if (strstr(command, "highlight_text") != NULL) {
        if (!highlight_txt(doc, command)) {
            bin = SUCCESS;
        }
    } else if (strstr(command, "remove_text") != NULL) {
        if (!remove_txt(doc, command)) {
            bin = SUCCESS;
        }
    } else if (strstr(command, "save_document") != NULL) {
        if (!save_doc(doc, command)) {
            bin = SUCCESS;
        }
    } else if (strstr(command, "reset_document") != NULL) {
        if (!reset_doc(doc, command)) {
            bin = SUCCESS;
        }
    }
    
    /* Handle invalid command case in case of function failures */
    if (bin == FAILURE) {
        printf("Invalid Command\n");
    }
    return bin;
}

/* Function to add a paragraph after a specified number */
int add_para_after(Document *doc, char *cmd) {
    char identifier[MAX_STR_SIZE + 1], extra[MAX_STR_SIZE + 1];
    int num_values, num;

    num_values = sscanf(cmd, "%s%d%s", identifier, &num, extra);
    if (num_values == 2 && num >= 0) {
        if (add_paragraph_after(doc, num) == FAILURE) {
            printf("add_paragraph_after failed\n");
        }
        return SUCCESS;
    }
    return FAILURE;
}

/* Function to add a line after a specified paragraph and line number */
int add_ln_after(Document *doc, char *cmd) {
    int l_val, i, j;
    char identifier[MAX_STR_SIZE + 1];
    char check_l[MAX_STR_SIZE + 1];
    char end[MAX_STR_SIZE + 1];

    l_val = sscanf(cmd, "%s%d%d%s", identifier, &i, &j, end);
    if (l_val == 4 && i > 0 && j >= 0) {
        if (strchr(cmd, '*') != NULL) {
            strcpy(check_l, strchr(cmd, '*') + 1);
            if (add_line_after(doc, i, j, check_l) == FAILURE) {
                printf("add_line_after failed\n");
            }
            return SUCCESS;
        }
    }
    return FAILURE;
}

/* Function to print the document information */
int print_doc(Document *doc, char *cmd) {
    int p_val;
    char identifier[MAX_STR_SIZE + 1], end[MAX_STR_SIZE + 1];

    p_val = sscanf(cmd, "%s%s", identifier, end);
    if (p_val == 1) {
        if (print_document(doc) == FAILURE) {
            printf("print_document failed\n");
        }
        return SUCCESS;
    }
    return FAILURE;
}

/* Function to append a line to a specified paragraph */
int append_ln(Document *doc, char *cmd) {
    int a_val, i;
    char identifier[MAX_STR_SIZE + 1], check_l[MAX_STR_SIZE + 1];
    char end[MAX_STR_SIZE + 1];

    a_val = sscanf(cmd, "%s%d%s", identifier, &i, end);
    if (a_val == 3 && i > 0) {
        if (strchr(cmd, '*') != NULL) {
            strcpy(check_l, strchr(cmd, '*') + 1);
            if (append_line(doc, i, check_l) == FAILURE) {
                printf("append_line failed\n");
            }
            return SUCCESS;
        }
    }
    return FAILURE;
}

/* Function to remove a line from a specified paragraph and line number */
int remove_ln(Document *doc, char *cmd) {
    int r_val, i, j;
    char identifier[MAX_STR_SIZE + 1], end[MAX_STR_SIZE + 1];

    r_val = sscanf(cmd, "%s%d%d%s", identifier, &i, &j, end);
    if (r_val == 3 && i > 0 && j > 0) {
        if (remove_line(doc, i, j) == FAILURE) {
            printf("remove_line failed\n");
        }
        return SUCCESS;
    }
    return FAILURE;
}

/* Function to load a document from a file */
int load_fl(Document *doc, char *cmd) {
    int l_val;
    char identifier[MAX_STR_SIZE + 1], file[MAX_STR_SIZE + 1];
    char end[MAX_STR_SIZE + 1];

    l_val = sscanf(cmd, "%s%s%s", identifier, file, end);
    if (l_val == 2) {
        if (load_file(doc, file) == FAILURE) {
            printf("load_file failed\n");
        }
        return SUCCESS;
    }
    return FAILURE;
}

/* Function to extract text enclosed in double quotes */
char *debrief(char *command, char *result) {
    char *i, *j;

    i = strstr(command, "\"");
    if (i != NULL) {
        j = strstr(i + 1, "\"");
        if (j != NULL) {
            strncpy(result, i + 1, j - i);
            result[j - (i + 1)] = '\0';
            return result;
        }
    }
    return NULL;
}

/* Function to replace text in the document */
int replace_txt(Document *doc, char *cmd) {
    char target[MAX_STR_SIZE + 1], replacement[MAX_STR_SIZE + 1];
    char *quoteStart = strstr(cmd, "\"");
    char *quoteEnd = quoteStart ? strstr(quoteStart + 1, "\"") : NULL;
    char *replaceStart = quoteEnd ? strstr(quoteEnd + 1, "\"") : NULL;
    char *replaceEnd = replaceStart ? strstr(replaceStart + 1, "\"") : NULL;

    if (quoteStart && quoteEnd && replaceStart && replaceEnd) {
        strncpy(target, quoteStart + 1, quoteEnd - quoteStart - 1);
        target[quoteEnd - (quoteStart + 1)] = '\0';
        strncpy(replacement, replaceStart + 1, replaceEnd - replaceStart - 1);
        replacement[replaceEnd - (replaceStart + 1)] = '\0';
        if (replace_text(doc, target, replacement) == FAILURE) {
            printf("replace_text failed\n");
            return FAILURE;
        }
        return SUCCESS;
    }
    printf("Invalid Command\n");
    return FAILURE;
}

/* Function to highlight text in the document */
int highlight_txt(Document *doc, char *cmd) {
    char identifier[MAX_STR_SIZE + 1], target[MAX_STR_SIZE + 1];
    char end[MAX_STR_SIZE + 1], *first, *result;
    int h_val;

    h_val = sscanf(cmd, "%s%s", identifier, end);
    first = strstr(cmd, "\"");
    if (h_val == 2 && first != NULL) {
        if ((result = debrief(first, target)) != NULL) {
            highlight_text(doc, result);
            return SUCCESS;
        }
    }
    return FAILURE;
}

/* Function to remove text from the document */
int remove_txt(Document *doc, char *cmd) {
    int r_val;
    char identifier[MAX_STR_SIZE + 1], check[MAX_STR_SIZE + 1];
    char temp[MAX_STR_SIZE + 1], *first, *result;

    r_val = sscanf(cmd, "%s%s", identifier, temp);
    first = strstr(cmd, "\"");
    if (r_val == 2 && first != NULL) {
        if ((result = debrief(first, check)) != NULL) {
            remove_text(doc, result);
            return SUCCESS;
        }
    }
    return FAILURE;
}

/* Function to save the document to a file */
int save_doc(Document *doc, char *cmd) {
    int save_v;
    char identifier[MAX_STR_SIZE + 1], file[MAX_STR_SIZE + 1];
    char end[MAX_STR_SIZE + 1];

    save_v = sscanf(cmd, "%s%s%s", identifier, file, end);
    if (save_v == 2) {
        if (save_document(doc, file) == FAILURE) {
            printf("save_document failed\n");
        }
        return SUCCESS;
    }
    return FAILURE;
}

/* Function to reset the document */
int reset_doc(Document *doc, char *cmd) {
    int r_val;
    char identifier[MAX_STR_SIZE + 1], end[MAX_STR_SIZE + 1];

    r_val = sscanf(cmd, "%s%s", identifier, end);
    if (r_val == 1) {
        reset_document(doc);
        return SUCCESS;
    }
    return FAILURE;
}
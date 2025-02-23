/* Rishi Erigineni
 */

#include "document.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

/* Initializes the document (pointed to by doc) to be empty and sets the
 * document's name to that provided by the parameter name.
 * An empty document has 0 paragraphs.
 */
int init_document(Document *doc, const char *name) {
  if (doc == NULL || name == NULL || strlen(name) > MAX_STR_SIZE) {
    return FAILURE;
  }
  strcpy(doc->name, name);
  doc->number_of_paragraphs = 0;

  return SUCCESS;
}

/* Sets the number of paragraphs in the document to 0. */
int reset_document(Document *doc) {
  if (doc == NULL) {
    return FAILURE;
  }
  doc->number_of_paragraphs = 0;

  return SUCCESS;
}

/* Prints the document's name, number of paragraphs,
 * and the paragraphs separated by blank lines.
 */
int print_document(Document *doc) {
  int i, j;

  if (doc == NULL) {
    return FAILURE;
  }
  printf("Document name: \"%s\"\n", doc->name);
  printf("Number of Paragraphs: %d\n", doc->number_of_paragraphs);

  for (i = 0; i < doc->number_of_paragraphs; i++) {
    for (j = 0; j < doc->paragraphs[i].number_of_lines; j++) {
      printf("%s\n", doc->paragraphs[i].lines[j]);
    }
    if (i < doc->number_of_paragraphs - 1 &&
        doc->paragraphs[i].number_of_lines > 0) {
      printf("\n");
    }
  }
  return SUCCESS;
}

/* Adds a paragraph after the specified paragraph number.
 * Paragraph numbers start at 1.
 */
int add_paragraph_after(Document *doc, int paragraph_number) {
  int index;

  if (doc == NULL || doc->number_of_paragraphs >= MAX_PARAGRAPHS ||
      paragraph_number < 0 ||
      paragraph_number > doc->number_of_paragraphs + 1) {
    return FAILURE;
  }

  if (doc->number_of_paragraphs == MAX_PARAGRAPHS) {
    return FAILURE;
  }
  if (paragraph_number < doc->number_of_paragraphs) {
    for (index = doc->number_of_paragraphs; index > paragraph_number; index--) {
      doc->paragraphs[index] = doc->paragraphs[index - 1];
    }
    doc->paragraphs[paragraph_number].number_of_lines = 0;
  } else {
    doc->paragraphs[doc->number_of_paragraphs].number_of_lines = 0;
  }

  (doc->number_of_paragraphs)++;
  return SUCCESS;
}

/* Adds a new line after the line with the specified line number. Line numbers
 * start at 1. */
int add_line_after(Document *doc, int paragraph_number, int line_number,
                   const char *new_line) {
  int i = 0, p_index = 0;
  Paragraph *paragraph;

  if (doc == NULL || paragraph_number <= 0 ||
      paragraph_number > doc->number_of_paragraphs || line_number < 0 ||
      line_number > MAX_PARAGRAPH_LINES - 1 || new_line == NULL) {
    return FAILURE;
  }

  p_index = paragraph_number - 1;
  paragraph = &doc->paragraphs[p_index];
  for (i = paragraph->number_of_lines; i > line_number; i--) {
    strcpy(paragraph->lines[i], paragraph->lines[i - 1]);
  }

  strcpy(paragraph->lines[line_number], new_line);
  paragraph->number_of_lines++;

  return SUCCESS;
}

/* Returns the number of lines in a paragraph using the number_of_lines out
 * parameter. */
int get_number_lines_paragraph(Document *doc, int paragraph_number,
                               int *number_of_lines) {
  if (doc == NULL || number_of_lines == NULL || paragraph_number <= 0 ||
      paragraph_number > doc->number_of_paragraphs) {
    return FAILURE;
  }
  *number_of_lines = doc->paragraphs[paragraph_number - 1].number_of_lines;

  return SUCCESS;
}

/* Returns the total number of lines in the document
 * using the number_of_lines out parameter.
 */
int get_number_lines(Document *doc, int *number_of_lines) {
  int i = 0, tl = 0;

  if (doc == NULL || number_of_lines == NULL) {
    return FAILURE;
  }
  for (i = 0; i < doc->number_of_paragraphs; i++) {
    tl += doc->paragraphs[i].number_of_lines;
  }
  *number_of_lines = tl;

  return SUCCESS;
}

/* Appends a line to the specified paragraph. */
int append_line(Document *doc, int paragraph_number, const char *new_line) {
  int paragraph_idx = paragraph_number - 1, line_number = 0;
  Paragraph *paragraph = &doc->paragraphs[paragraph_idx];

  if (doc == NULL || paragraph_number <= 0 ||
      paragraph_number > doc->number_of_paragraphs || new_line == NULL) {
    return FAILURE;
  }
  if (paragraph->number_of_lines >= MAX_PARAGRAPH_LINES) {
    return FAILURE;
  }
  line_number = paragraph->number_of_lines;

  strcpy(paragraph->lines[line_number], new_line);
  paragraph->number_of_lines++;

  return SUCCESS;
}

/* Removes the specified line from the paragraph. */
int remove_line(Document *doc, int paragraph_number, int line_number) {
  int i, j, num_lines = 0;
  char *curr, *curr_l;
  Paragraph *paragraph;

  if (doc == NULL || paragraph_number <= 0 ||
      paragraph_number > doc->number_of_paragraphs) {
    return FAILURE;
  }
  num_lines = doc->paragraphs[paragraph_number - 1].number_of_lines;

  if (line_number <= 0 || line_number > num_lines) {
    return FAILURE;
  }
  if (line_number < num_lines) {
    paragraph = &doc->paragraphs[paragraph_number - 1];
    for (i = line_number - 1; i < num_lines - 1; i++) {
      curr = paragraph->lines[i];
      curr_l = paragraph->lines[i + 1];
      for (j = 0; j < MAX_STR_SIZE; j++) {
        curr[j] = curr_l[j];
      }
    }
  }
  (doc->paragraphs[paragraph_number - 1].number_of_lines)--;
  return SUCCESS;
}

/* Adds the first data_lines number of lines from the data array to the
 * document. An empty string in the array creates a new paragraph.
 */
int load_document(Document *doc, char data[][MAX_STR_SIZE + 1],
                  int data_lines) {
  int i;

  if (doc == NULL || data == NULL || !data_lines) {
    return FAILURE;
  }
  add_paragraph_after(doc, doc->number_of_paragraphs);
  for (i = 0; i < data_lines; i++) {
    if (strcmp(data[i], "") == 0) {
      add_paragraph_after(doc, doc->number_of_paragraphs);
    } else {
      append_line(doc, doc->number_of_paragraphs, data[i]);
    }
  }
  return SUCCESS;
}

/* Replaces the text target with the text replacement everywhere it appears in
 * the document. */
int replace_text(Document *doc, const char *target, const char *replacement) {
  int p_index, l_index, nl = 0;
  char *line, *curr, place[MAX_STR_SIZE + 1] = "";

  if (doc == NULL || target == NULL || replacement == NULL) {
    return FAILURE;
  }

  for (p_index = 0; p_index < doc->number_of_paragraphs; p_index++) {
    nl = doc->paragraphs[p_index].number_of_lines;
    for (l_index = 0; l_index < nl; l_index++) {
      line = doc->paragraphs[p_index].lines[l_index];
      curr = strstr(line, target);
      while (curr != NULL) {
        strncpy(place, line, curr - line);
        place[curr - line] = '\0';
        strcat(place, replacement);
        strcat(place, curr + strlen(target));
        strcpy(line, place);
        curr = strstr(curr + strlen(replacement), target);
      }
    }
  }
  return SUCCESS;
}

/* Highlights the text associated with target everywhere it appears in the
 * document. */
int highlight_text(Document *doc, const char *target) {
  char temp[MAX_STR_SIZE + 1] = "";

  if (doc == NULL || target == NULL) {
    return FAILURE;
  }
  strcat(temp, HIGHLIGHT_START_STR);
  strcat(temp, target);
  strcat(temp, HIGHLIGHT_END_STR);
  replace_text(doc, target, temp);
  return SUCCESS;
}

/* Removes the text target everywhere it appears in the document. */
int remove_text(Document *doc, const char *target) {
  if (doc == NULL || target == NULL) {
    return FAILURE;
  }

  replace_text(doc, target, "");
  return SUCCESS;
}

/* Load the content of a file into the document */
int load_file(Document *doc, const char *filename) {
  FILE *file;
  int current_paragraph_number;
  char current_line[MAX_STR_SIZE + 1], first_char,
      rest_of_line[MAX_STR_SIZE + 1];

  /* Check for NULL pointers in the input arguments */
  if (!doc || !filename) {
    return FAILURE;
  }

  /* Attempt to open the file for reading */
  if ((file = fopen(filename, "r")) == NULL) {
    return FAILURE;
  }

  /* Initialize paragraph number and add an initial paragraph */
  current_paragraph_number = 1;
  add_paragraph_after(doc, 0);

  /* Read lines from the file */
  while (fgets(current_line, sizeof(current_line), file)) {
    /* Check if the line starts with a single character followed by some text */
    if (sscanf(current_line, " %c%80s", &first_char, rest_of_line) < 2) {
      /* Add a new paragraph and update the paragraph number */
      add_paragraph_after(doc, current_paragraph_number);
      current_paragraph_number++;
    } else if (first_char != '#' &&
               current_line[strlen(current_line) - 1] == '\n') {
      /* Trim the newline character and append the line to the current paragraph */
      current_line[strlen(current_line) - 1] = '\0';
      append_line(doc, current_paragraph_number, current_line);
    }
  }

  /* Close the file after reading */
  fclose(file);
  return SUCCESS;
}

/* Save the content of the document to a file */
int save_document(Document *doc, const char *filename) {
  FILE *s_file;
  int i, j;

  /* Check for NULL pointers in the input arguments */
  if (doc == NULL || filename == NULL) {
    return FAILURE;
  }

  /* Attempt to open the file for writing */
  s_file = fopen(filename, "w");
  if (s_file == NULL) {
    return FAILURE;
  }

  /* Iterate through paragraphs and lines, writing to the file */
  for (i = 0; i < doc->number_of_paragraphs; i++) {
    for (j = 0; j < doc->paragraphs[i].number_of_lines; j++) {
      /* Write each line of the paragraph to the file */
      fprintf(s_file, "%s\n", doc->paragraphs[i].lines[j]);
    }

    /* Add a newline between paragraphs, except for the last paragraph */
    if (i < doc->number_of_paragraphs - 1 &&
        doc->paragraphs[i].number_of_lines > 0) {
      fprintf(s_file, "\n");
    }
  }

  /* Close the file after writing */
  fclose(s_file);
  return SUCCESS;
}

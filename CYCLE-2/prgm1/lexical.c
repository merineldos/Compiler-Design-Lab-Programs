#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LEN 100

const char *keywords[] = {
    "auto", "break", "case", "char", "const", "continue", "default", "do",
    "double", "else", "enum", "extern", "float", "for", "goto", "if",
    "inline", "int", "long", "register", "restrict", "return", "short",
    "signed", "sizeof", "static", "struct", "switch", "typedef", "union",
    "unsigned", "void", "volatile", "while"
};

int isKeyword(char *str) {
    int arraySize = sizeof(keywords) / sizeof(keywords[0]);
    for (int i = 0; i < arraySize; i++) {
        if (strcmp(str, keywords[i]) == 0)
            return 1;
    }
    return 0;
}

int isDelimiter(char ch) {
    return (ch == ';' || ch == ',' || ch == '(' || ch == ')' ||
            ch == '{' || ch == '}' || ch == '[' || ch == ']');
}

int isOperator(char ch) {
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/' ||
            ch == '=' || ch == '<' || ch == '>');
}

void skipSingleLineComment(FILE *fp) {
    int ch;
    while ((ch = fgetc(fp)) != EOF && ch != '\n');
}

void skipMultiLineComment(FILE *fp) {
    int ch, prev = 0;
    while ((ch = fgetc(fp)) != EOF) {
        if (prev == '*' && ch == '/')
            break;
        prev = ch;
    }
}

void lexicalAnalyzer(FILE *fp) {
    int c, i, tokenCount = 0;
    char buffer[MAX_LEN];

    while ((c = fgetc(fp)) != EOF) {
        char ch = (char)c;

        if (isspace(ch)) continue;

        // --- Identifiers / Keywords ---
        if (isalpha(ch) || ch == '_') {
            i = 0;
            buffer[i++] = ch;
            int next_c;
            while ((next_c = fgetc(fp)) != EOF && (isalnum(next_c) || next_c == '_')) {
                buffer[i++] = (char)next_c;
            }
            buffer[i] = '\0';
            if (next_c != EOF) ungetc(next_c, fp);

            if (isKeyword(buffer)) printf("<KEYWORD, %s>\n", buffer);
            else printf("<IDENTIFIER, %s>\n", buffer);

            tokenCount++;
        }

        // --- Numbers (integers only) ---
        else if (isdigit(ch)) {
            i = 0;
            buffer[i++] = ch;
            int next_c;
            while ((next_c = fgetc(fp)) != EOF && isdigit(next_c)) {
                buffer[i++] = (char)next_c;
            }
            buffer[i] = '\0';
            if (next_c != EOF) ungetc(next_c, fp);

            printf("<NUMBER, %s>\n", buffer);
            tokenCount++;
        }

        // --- Comments ---
        else if (ch == '/') {
            int next_c = fgetc(fp);
            if (next_c == '/') {
                skipSingleLineComment(fp);
            } else if (next_c == '*') {
                skipMultiLineComment(fp);
            } else {
                if (next_c != EOF) ungetc(next_c, fp);
                printf("<OPERATOR, %c>\n", ch);
                tokenCount++;
            }
        }

        // --- Operators (single char only) ---
        else if (isOperator(ch)) {
            printf("<OPERATOR, %c>\n", ch);
            tokenCount++;
        }

        // --- Delimiters ---
        else if (isDelimiter(ch)) {
            printf("<DELIMITER, %c>\n", ch);
            tokenCount++;
        }

        // --- Preprocessor Directives ---
        else if (ch == '#') {
            i = 0;
            buffer[i++] = ch;
            int next_c;
            while ((next_c = fgetc(fp)) != EOF && next_c != '\n') {
                buffer[i++] = (char)next_c;
            }
            buffer[i] = '\0';
            printf("<PREPROCESSOR, %s>\n", buffer);
            tokenCount++;
        }
    }

    printf("\nTotal tokens processed: %d\n", tokenCount);
}

int main() {
    FILE *fp = fopen("source.txt", "r");
    if (!fp) {
        printf("Error: source.txt not found!\n");
        return 1;
    }
    printf("Tokens identified:\n");
    lexicalAnalyzer(fp);
    fclose(fp);
    return 0;
}

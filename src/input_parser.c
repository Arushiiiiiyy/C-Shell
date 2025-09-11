#include "../include/headers.h"
#define MAX_TOKENS 4096
#define MAX_LEN 1024

//THE CODE BELOW IS CORRECT, IF YOURS DOESNT WORK , USE THIS 
char *token_store[MAX_TOKENS];
int token_store_index = 0;

bool is_name(const char *tok) {
    // A name cannot contain | & > < ;
    if (strpbrk(tok, "|&><;")) return false;
    return strlen(tok) > 0;
}

bool parse_atomic(char **tokens, int *pos, int ntokens) {
    if (*pos >= ntokens || !is_name(tokens[*pos])) return false;
    (*pos)++; // consume first name

    while (*pos < ntokens) {
        char *tok = tokens[*pos];

        if (is_name(tok)) {
            // another plain name
            (*pos)++;
        } 
        else if (strcmp(tok, "<") == 0) {
            // case: "< name"
            (*pos)++;
            if (*pos >= ntokens || !is_name(tokens[*pos])) return false;
            (*pos)++;
        } 
        else if (tok[0] == '<') {
            // case: "<name" (no space)
            if (strlen(tok) == 1) return false; // just "<" alone handled above
            (*pos)++;
        }
        else if (strcmp(tok, ">") == 0 || strcmp(tok, ">>") == 0) {
            // case: "> name" or ">> name"
            (*pos)++;
            if (*pos >= ntokens || !is_name(tokens[*pos])) return false;
            (*pos)++;
        } 
        else if (tok[0] == '>') {
            // case: ">name" or ">>name" (no space)
            if (tok[1] == '\0') return false; // just ">" alone handled above
            (*pos)++;
        }
        else {
            break; // atomic ends
        }
    }
    return true;
}

bool parse_cmd_group(char **tokens, int *pos, int ntokens) {
    if (!parse_atomic(tokens, pos, ntokens)) return false;

    while (*pos < ntokens && strcmp(tokens[*pos], "|") == 0) {
        (*pos)++; // consume '|'
        if (!parse_atomic(tokens, pos, ntokens)) return false;
    }
    return true;
}

bool parse_shell_cmd(char **tokens, int ntokens) {
    int pos = 0;
    if (!parse_cmd_group(tokens, &pos, ntokens)) return false; // check first one

    while (pos < ntokens &&
          (strcmp(tokens[pos], ";") == 0 || strcmp(tokens[pos], "&") == 0)) {
        char *sep = tokens[pos++];
        if (pos < ntokens && (strcmp(sep, ";") == 0 || strcmp(sep, "&") == 0)) {
            if (!parse_cmd_group(tokens, &pos, ntokens)) return false;
        }
    }

    // optional trailing &
    if (pos < ntokens && strcmp(tokens[pos], "&") == 0) pos++;

    return pos == ntokens;
}

// ---------- tokenizer + parser wrapper ----------

// ---------- tokenizer + parser wrapper ----------

bool input_parser(char *input, int input_length) {
    char *tok = strtok(input, " \t\n");
    token_store_index = 0;

    while (tok != NULL && token_store_index < MAX_TOKENS) {
        token_store[token_store_index++] = tok;
        tok = strtok(NULL, " \t\n");
    }

    if (token_store_index == 0) return false;  // empty input treated as invalid

    if (parse_shell_cmd(token_store, token_store_index)) {
        // valid syntax: do nothing, just return true
        return true;
    } else {
        // invalid syntax: print error
        printf("Invalid Syntax!\n");
        fflush(stdout);
        return false;
    }
}


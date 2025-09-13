//LLM GENERATED CODE 

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
    token_store_index = 0;

    char buf[MAX_LEN];
    int b = 0;
    for (int i = 0; i < input_length; i++) {
        char c = input[i];

        // whitespace => flush current token
        if (isspace((unsigned char)c)) {
            if (b > 0) {
                buf[b] = '\0';
                token_store[token_store_index++] = strdup(buf);
                b = 0;
            }
            continue;
        }

        // single-char punctuation => flush + add as own token
        if (c=='|' || c=='&' || c==';' || c=='<' || c=='>') {
            if (b > 0) {
                buf[b] = '\0';
                token_store[token_store_index++] = strdup(buf);
                b = 0;
            }

            // handle >> specially
            if (c=='>' && i+1 < input_length && input[i+1]=='>') {
                token_store[token_store_index++] = strdup(">>");
                i++; // skip next
            } else {
                char t[2] = {c,'\0'};
                token_store[token_store_index++] = strdup(t);
            }
            continue;
        }

        // normal character
        buf[b++] = c;
    }

    if (b > 0) {
        buf[b] = '\0';
        token_store[token_store_index++] = strdup(buf);
    }

    if (token_store_index == 0) return false;

    if (parse_shell_cmd(token_store, token_store_index)) {
        return true;
    } else {
        printf("Invalid Syntax!\n");
        fflush(stdout);
        return false;
    }
}

//LLM GENERATED CODE ENDED
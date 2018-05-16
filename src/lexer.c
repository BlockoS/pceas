#include "lexer.h"

// [todo] error messages

static int space_token(struct parser_t *parser, int c) {
    do {
        int ret = string_push(&parser->value, (char)c);
        if(ret) { return ret; }
        ret = parser_getc(parser, &c);
        if(ret) { return ret; }
    } while((c == ' ') || (c == '\t'));
    
    parser->token = ' ';
    return parser_ungetc(parser, &c);
}

static int comment_token(struct parser_t *parser, int c) {
    do {
        int ret = string_push(&parser->value, (char)c);
        if(ret) { return ret; }
        ret = parser_getc(parser, &c);
        if(ret) { return ret; }
    } while((c != '\n') && (c != '\r'));
    
    parser->token = ';';
    return parser_ungetc(parser, &c);
}

static int newline_token(struct parser_t *parser, int c) {
    int ret;
    ret = string_push(&parser->value, (char)c);
    if(ret) { return ret; }
    
    ret = parser_getc(parser, &c);
    if(ret) { return ret; }
    
    parser->line++;
    if(((c == '\n') || (c == '\r')) && (c != string_last(&parser->value))) {
        ret = string_push(&parser->value, (char)c);
    } else {
        ret = parser_ungetc(parser, &c);
    }
    parser->token = '\n';
    return ret;
}

static int identifier_token(struct parser_t *parser, int c) {
    int ret;
    do {
        ret = string_push(&parser->value, (char)c);
        if(ret) { return ret; }
    
        ret = parser_getc(parser, &c);
        if(ret) { return ret; }
    } while( ((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z')) || (c == '.') || (c == '_') );

    ret = parser_ungetc(parser, &c);
    if(ret) { return ret; }

    parser->token = IDENTIFIER_TOKEN;
    return 0;
}

static int number_token(struct parser_t *parser, int c) {
    int ret;
    do {
        ret = string_push(&parser->value, (char)c);
        if(ret) { return ret; }
    
        ret = parser_getc(parser, &c);
        if(ret) { return ret; }
    } while( ((c >= '0') && (c <= '9')) || ((c >= 'a') && (c <= 'f')) || ((c >= 'A') && (c <= 'F')) || (c == '_') );

    ret = parser_ungetc(parser, &c);
    if(ret) { return ret; }

    parser->token = NUMBER_TOKEN;
    return 0;
}

static int tokenize(struct parser_t *parser) {
    int c = '\0';
    int ret = 0;
    parser->token = NONE_TOKEN;

    ret = parser_getc(parser, &c);
    if(ret) { return ret; }

    string_clear(&parser->value);
    if((c >= '0') && (c <= '9')) {
        ret = number_token(parser, c);
    } else if( ((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z')) || (c == '_') ) {
        ret = identifier_token(parser, c);
    } else switch(c) {
        case '#':
        case '\'':
        case '"':
        case '=':
        case '(':
        case ')':
        case '[':
        case ']':
        case '{':
        case '}':
        case '.':
        case '@':
        case '-':
        case '+': 
        case '/':
        case '*':
        case '$':
        case '%':
        case '&':
        case '^':
        case '|':
        case '<':
        case '>':
            parser->token = c;
            ret = string_push(&parser->value, (char)c);
            break;
        case ';':
            ret = comment_token(parser, c);
            break;
        case EOF:
            parser->token = c;
            break;
        case '\t': case ' ':
            ret = space_token(parser, c);
            break;        
        case '\r': case '\n':
            ret = newline_token(parser, c);
            break;
        default:
            parser->token = ERROR_TOKEN;
            break;
    }   
    return ret;
}

static int skip_token(token_t t) {
    return (t == ' ') || (t == ';');
}

int next_token(struct parser_t *in, struct string_t *out, token_t *token) {
    int ret;
    ret = string_copy(out, &in->value);
    if(ret) {
        return ret;
    }
    *token = in->token;
    do {
        ret = tokenize(in);
    } while(!ret && skip_token(in->token));
    return ret;
}


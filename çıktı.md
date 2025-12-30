-- Ana tokenizer
-- -----------------------------------------------------------------------------

function get_next_token(state)
    if state.eof_reached == 1 then
        return make_token(TOKEN_EOF; ""; state.line; state.column)
    end_if
    
    state = skip_whitespace_and_comments(state)
    
    if is_eof(state) == 1 then
        state.eof_reached = 1
        return make_token(TOKEN_EOF; ""; state.line; state.column)
    end_if
    
    String ch = ""
    ch = current_char(state)
    
    if is_digit(ch) == 1 then
        return read_number(state)
    end_if
    
    if ch == "\"" then
        return read_metin(state; "\"")
    end_if
    
    if ch == "'" then
        return read_metin(state; "'")
    end_if
    
    if is_alpha(ch) == 1 then
        return read_identifier(state)
    end_if
    
    return read_operator(state)
end_function

function tokenize(source_code)
    LexerState state
    state = lexer_state_create(source_code)
    TokenStream stream
    stream = token_stream_create(list())
    
    while state.eof_reached == 0
        Token token
        token = get_next_token(state)
        mlp_list_add(stream.tokens; token)
        
        if token.type == TOKEN_EOF then
            break
        end_if
    end_while
    
    return stream
end_function

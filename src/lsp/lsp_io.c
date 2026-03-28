/*
 * lsp_io.c — MELP LSP Server C runtime yardımcıları
 *
 * MLP'nin Stage0 derleyicisi stdin'den okuma yapamıyor (input() parse edilemiyor).
 * Bu C kütüphanesi MLP external function'ları olarak stdin/stdout işlemlerini sağlar.
 *
 * MLP tarafından kullanılan extern fonksiyonlar:
 *   lsp_getline()     → stdin'den bir satır okur (trailing \n dahil DEĞİL)
 *                       EOF'ta "" (boş string) döner
 *   lsp_write(s)      → stdout'a yazar (flush ile)
 *   lsp_write_err(s)  → stderr'e yazar (debug log için)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * lsp_getline() → string
 * stdin'den bir satır okur. Trailing \n ve \r kaldırılır.
 * EOF veya hata durumunda boş string döner.
 *
 * Not: MLP runtime char* pointer'ı doğrudan string olarak kullanır.
 * strdup ile heap'e kopyalıyoruz — runtime bunu free() ile serbest bırakır.
 */
char* lsp_getline(void) {
    char* line = NULL;
    size_t len = 0;
    ssize_t nread = getline(&line, &len, stdin);

    if (nread == -1) {
        if (line) free(line);
        return strdup("");
    }

    /* Remove trailing \r\n or \n */
    while (nread > 0 && (line[nread-1] == '\n' || line[nread-1] == '\r')) {
        line[--nread] = '\0';
    }

    char* result = strdup(line);
    free(line);
    return result;
}

/*
 * lsp_send_message(body) → numeric (0 = ok)
 * LSP stdout framing: "Content-Length: N\r\n\r\n{body}" + flush.
 * MLP'de void fonksiyon codegen sorunu olduğu için numeric döner.
 */
long long lsp_send_message(const char* body) {
    if (!body) return -1;
    size_t blen = strlen(body);
    /* Content-Length header + CRLF CRLF + body */
    fprintf(stdout, "Content-Length: %zu\r\n\r\n%s", blen, body);
    fflush(stdout);
    return 0;
}

/*
 * lsp_log(s) → numeric (0 = ok)
 * stderr'e log yazar. stdout LSP mesajları için ayrılmış.
 */
long long lsp_log(const char* s) {
    if (s) {
        fputs(s, stderr);
        fputs("\n", stderr);
        fflush(stderr);
    }
    return 0;
}

/*
 * lsp_eof() → numeric (1 = EOF)
 */
long long lsp_eof(void) {
    return feof(stdin) ? 1 : 0;
}

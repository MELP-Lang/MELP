#ifndef NORMALIZE_H
#define NORMALIZE_H

/**
 * PMPL Normalizer
 * 
 * User input'u PMPL syntax'ına dönüştürür.
 * 
 * Örnek:
 *   Input:  "if x > 5 then\n    print(x)\nend if"
 *   Output: "if x > 5 then\n    print(x)\nend_if"
 */

/**
 * User input string'ini PMPL'ye normalize eder.
 * 
 * @param input Kullanıcı kodu (herhangi bir syntax)
 * @return PMPL syntax'ına dönüştürülmüş string (caller free etmeli)
 */
char* normalize_to_pmpl(const char* input);

/**
 * Helper: Bir string'de belirli pattern'i underscore versiyonu ile değiştir
 * 
 * @param input Kaynak string
 * @param from Pattern (örn: "end if")
 * @param to Hedef (örn: "end_if")
 * @return Yeni string (caller free etmeli)
 */
char* replace_pattern(const char* input, const char* from, const char* to);

#endif // NORMALIZE_H

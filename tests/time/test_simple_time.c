#include <time.h>
#include <stdio.h>

int main() {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    printf("Year: %d\n", tm_info->tm_year + 1900);
    return 0;
}

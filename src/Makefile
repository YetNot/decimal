flags = -c -Wall -Wextra -Werror -std=c11
TST_LIBS = -lcheck -lm -lpthread
COV_LIBS = -lgcov -coverage
GCONV_FLAGS = . --html --html-details 
OTHER = gcov -abcfu test.c
PROFILE_FLAGS = -fprofile-arcs -ftest-coverage

all: clean s21_decimal.a

all_peer: clean s21_decimal_peer test gcov_report 

clean: 
	rm -rf *.o *.a *.gcda *.gcno check_tests *.html *.css *.out main

test: s21_decimal_peer
	gcc $(flags) test.c
	gcc test.o -L. s21_decimal.a $(TST_LIBS) $(COV_LIBS) -o check_tests
	./check_tests

s21_decimal.a: 
	gcc $(flags) bool_special.c bool.c common.c converter.c math_special.c math.c round.c
	ar rc s21_decimal.a *.o
	ranlib s21_decimal.a

s21_decimal_peer: 
	gcc $(flags) bool_special.c common.c math_special.c
	gcc $(flags) $(PROFILE_FLAGS) bool.c converter.c math.c round.c
	ar rc s21_decimal.a *.o
	ranlib s21_decimal.a

gcov_report: test
	gcovr $(GCONV_FLAGS) -o gcov_report.html
	open gcov_report.html

main: s21_decimal.a
	gcc $(flags) main.c
	gcc main.o -L. s21_decimal.a $(TST_LIBS) $(COV_LIBS)

style:
	clang-format -n *.c *.h

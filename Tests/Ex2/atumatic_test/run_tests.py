#  Orsha

from tests import test_Caesar_1, test_Caesar_2 ,\
    test_Caesar_3 , test_Caesar_4 , test_Caesar_5 ,\
    test_Caesar_6 , test_Caesar_7, test_Caesar_8 ,\
    test_Caesar_9 ,test_Caesar_10 ,test_Caesar_11

def run_all():

    tests_apss = [test_Caesar_1(),
                  test_Caesar_2(),
                  test_Caesar_3(),
                  test_Caesar_4(),
                  test_Caesar_5(),
                  test_Caesar_6(),
                  test_Caesar_7(),
                  test_Caesar_8(),
                  test_Caesar_9(),
                  test_Caesar_10()
                  ,test_Caesar_11()]

    for test in tests_apss:
        test.run()

if __name__ == "__main__":
    run_all()
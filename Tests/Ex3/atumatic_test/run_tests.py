#  Orsha
import os

from tests import test_1,test_2, test_3

def run_all():

    tests_apss = [test_1(),
                  test_2(),
                  test_3()]

    for test in tests_apss:
        test.run()
        raw_input("Press Enter to continue...")
        os.system("taskkill /im notepad.exe")


if __name__ == "__main__":
    run_all()
#  Orsha

import os
import filecmp
from base_test import test

OKGREEN = '\033[92m'
OKBLUE = '\033[94m'
ENDC = '\033[0m'
BOLD = "\033[1m"

HEADER = '\033[95m'
WARNING = '\033[93m'
FAIL = '\033[91m'

PATH_TO_PROJECTS = "C:\Users\User\Desktop\os\IPS_Projects"
PATH_TO_APP = PATH_TO_PROJECTS+"\IPS_Ex2\Debug\Caesar.exe "
PATH_TO_INPUT_FILE = PATH_TO_PROJECTS + "\Tests\Ex2\\"


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#                                                 Test 1
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class test_Caesar_1 (test):
    def __init__(self):
        super(test_Caesar_1 ,self).__init__(1)

    def run (self):
        thrads_num = 15
        key = 6
        print (BOLD + "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" + ENDC )
        print(OKBLUE + " Test " + str(self.test_number)  + ENDC)
        print(OKBLUE + " [key  = {key}, thrads_num = {thrads_num} ]".format(key = key,thrads_num = thrads_num ) + ENDC)

        print(HEADER + " Loading App step 1"  + ENDC)
        os.system(PATH_TO_APP + PATH_TO_INPUT_FILE + "plaintext_example.txt " + str(key ) + " " + str(thrads_num) +" -d")

        print(HEADER + " Loading App step 2"  + ENDC)
        os.system(PATH_TO_APP + PATH_TO_INPUT_FILE + "decrypted.txt " + str(key ) + " " + str(thrads_num) +" -e")

        print(HEADER + " Comapre " + ENDC)
        if (filecmp.cmp(PATH_TO_INPUT_FILE + "plaintext_example.txt ", PATH_TO_INPUT_FILE + "encrypted.txt")):
            print(OKGREEN + " PASS  - TEST" +  ENDC )
        else:
            print (FAIL + " FALIED" + ENDC)

        print('\n')


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#                                                  Test 2
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class test_Caesar_2 (test):
    def __init__(self):
        super(test_Caesar_2, self).__init__(2)

    def run (self):
        thrads_num = 22
        key = -16
        print (BOLD + "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" + ENDC )
        print(OKBLUE + " Test " + str(self.test_number) + ENDC)
        print(OKBLUE + " [key  = {key}, thrads_num = {thrads_num} ]".format(key = key,thrads_num = thrads_num ) + ENDC)

        print(HEADER + " Loading App step 1"  + ENDC)
        os.system(PATH_TO_APP + PATH_TO_INPUT_FILE + "plaintext_example.txt " + str(key ) + " " + str(thrads_num) +" -d")

        print(HEADER + " Loading App step 2"  + ENDC)
        os.system(PATH_TO_APP + PATH_TO_INPUT_FILE + "decrypted.txt " + str(key ) + " " + str(thrads_num) +" -e")

        print(HEADER + " Comapre " + ENDC)
        if (filecmp.cmp(PATH_TO_INPUT_FILE + "plaintext_example.txt ", PATH_TO_INPUT_FILE + "encrypted.txt")):
            print(OKGREEN + " PASS  - TEST" +   ENDC )
        else:
            print (FAIL + "FALIED" + ENDC)

        print('\n')

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#                                                   Test 3
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class test_Caesar_3 (test):
    def __init__(self):
        super(test_Caesar_3, self).__init__(3)

    def run (self):
        thrads_num = 26
        key = -16
        print (BOLD + "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" + ENDC )
        print(OKBLUE + " Test " + str(self.test_number)  + ENDC)
        print(OKBLUE + " [key  = {key}, thrads_num = {thrads_num} ]".format(key = key,thrads_num = thrads_num ) + ENDC)

        print(HEADER + " Loading App step 1"  + ENDC)
        os.system(PATH_TO_APP + PATH_TO_INPUT_FILE + "plaintext_example.txt " +str(key ) + " " + str(thrads_num) +" -d")

        print(HEADER + " Loading App step 2"  + ENDC)
        os.system(PATH_TO_APP + PATH_TO_INPUT_FILE + "decrypted.txt " + str(key ) + " " + str(thrads_num) +" -e")

        print(HEADER + " Comapre " + ENDC)
        if (filecmp.cmp(PATH_TO_INPUT_FILE + "plaintext_example.txt ", PATH_TO_INPUT_FILE + "encrypted.txt")):
            print(OKGREEN + " PASS  - TEST" +   ENDC )
        else:
            print (FAIL + "FALIED" + ENDC)

        print('\n')



# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#                                                   Test 4
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class test_Caesar_4 (test):
    def __init__(self):
        super(test_Caesar_4, self).__init__(4)

    def run (self):
        thrads_num = 3
        key = 116
        print (BOLD + "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" + ENDC )
        print(OKBLUE + " Test " + str(self.test_number)  + ENDC)
        print(OKBLUE + " [key  = {key}, thrads_num = {thrads_num} ]".format(key = key,thrads_num = thrads_num ) + ENDC)

        print(HEADER + " Loading App step 1"  + ENDC)
        os.system(PATH_TO_APP + PATH_TO_INPUT_FILE + "plaintext_example.txt " + str(key ) + " " + str(thrads_num) +" -d")

        print(HEADER + " Loading App step 2"  + ENDC)
        os.system(PATH_TO_APP + PATH_TO_INPUT_FILE + "decrypted.txt " + str(key ) + " " + str(thrads_num) +" -e")

        print(HEADER + " Comapre " + ENDC)
        if (filecmp.cmp(PATH_TO_INPUT_FILE + "plaintext_example.txt ", PATH_TO_INPUT_FILE + "encrypted.txt")):
            print(OKGREEN + " PASS  - TEST" +   ENDC )
        else:
            print (FAIL + "FALIED" + ENDC)

        print('\n')



# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#                                                   Test 5
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class test_Caesar_5 (test):
    def __init__(self):
        super(test_Caesar_5, self).__init__(5)

    def run (self):
        thrads_num = 5
        key = -116
        print (BOLD + "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" + ENDC )
        print(OKBLUE + " Test " + str(self.test_number)  + ENDC)
        print(OKBLUE + " [key  = {key}, thrads_num = {thrads_num} ]".format(key = key,thrads_num = thrads_num ) + ENDC)

        print(HEADER + " Loading App step 1"  + ENDC)
        os.system(PATH_TO_APP + PATH_TO_INPUT_FILE + "plaintext_example.txt " + str(key ) + " " + str(thrads_num) + " -d")

        print(HEADER + " Loading App step 2"  + ENDC)
        os.system(PATH_TO_APP + PATH_TO_INPUT_FILE + "decrypted.txt " + str(key ) + " " + str(thrads_num) +" -e")

        print(HEADER + " Comapre " + ENDC)
        if (filecmp.cmp(PATH_TO_INPUT_FILE + "plaintext_example.txt ", PATH_TO_INPUT_FILE + "encrypted.txt")):
            print(OKGREEN + " PASS  - TEST" +   ENDC )
        else:
            print (FAIL + "FALIED" + ENDC)

        print('\n')



# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#                                                   Test 6
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class test_Caesar_6 (test):
    def __init__(self):
        super(test_Caesar_6, self).__init__(6)

    def run (self):
        thrads_num = 6
        key = -116
        print (BOLD + "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" + ENDC )
        print(OKBLUE + " Test " + str(self.test_number)  + ENDC)
        print(OKBLUE + " [key  = {key}, thrads_num = {thrads_num} ]".format(key = key,thrads_num = thrads_num ) + ENDC)

        print(HEADER + " Loading App step 1"  + ENDC)
        os.system(PATH_TO_APP + PATH_TO_INPUT_FILE + "empty_file.txt " + str(key ) + " " + str(thrads_num) + " -d")

        print(HEADER + " Loading App step 2"  + ENDC)
        os.system(PATH_TO_APP + PATH_TO_INPUT_FILE + "decrypted.txt " + str(key ) + " " + str(thrads_num) +" -e")

        print(HEADER + " Comapre " + ENDC)
        if (filecmp.cmp(PATH_TO_INPUT_FILE + "empty_file.txt ", PATH_TO_INPUT_FILE + "encrypted.txt")):
            print(OKGREEN + " PASS  - TEST" +   ENDC )
        else:
            print (FAIL + "FALIED" + ENDC)

        print('\n')




# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#                                                   Test 7
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class test_Caesar_7 (test):
    def __init__(self):
        super(test_Caesar_7, self).__init__(7)

    def run (self):
        thrads_num = 50
        key = 6
        print (BOLD + "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" + ENDC )
        print(OKBLUE + " Test " + str(self.test_number)  + ENDC)
        print(OKBLUE + " [key  = {key}, thrads_num = {thrads_num} ]".format(key = key,thrads_num = thrads_num ) + ENDC)

        print(HEADER + " Loading App step 1"  + ENDC)
        os.system(PATH_TO_APP + PATH_TO_INPUT_FILE + "file_test_1.txt " + str(key ) + " " + str(thrads_num) + " -d")

        print(HEADER + " Loading App step 2"  + ENDC)
        os.system(PATH_TO_APP + PATH_TO_INPUT_FILE + "decrypted.txt " + str(key ) + " " + str(thrads_num) +" -e")

        print(HEADER + " Comapre " + ENDC)
        if (filecmp.cmp(PATH_TO_INPUT_FILE + "file_test_1.txt ", PATH_TO_INPUT_FILE + "encrypted.txt")):
            print(OKGREEN + " PASS  - TEST" +   ENDC )
        else:
            print (FAIL + "FALIED" + ENDC)

        print('\n')



# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#                                                   Test 8
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class test_Caesar_8 (test):
    def __init__(self):
        super(test_Caesar_8, self).__init__(8)

    def run (self):
        thrads_num = 60
        key = -1629
        print (BOLD + "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" + ENDC )
        print(OKBLUE + " Test " + str(self.test_number)  + ENDC)
        print(OKBLUE + " [key  = {key}, thrads_num = {thrads_num} ]".format(key = key,thrads_num = thrads_num ) + ENDC)

        print(HEADER + " Loading App step 1"  + ENDC)
        os.system(PATH_TO_APP + PATH_TO_INPUT_FILE + "file_test_1.txt " + str(key ) + " " + str(thrads_num) + " -d")

        print(HEADER + " Loading App step 2"  + ENDC)
        os.system(PATH_TO_APP + PATH_TO_INPUT_FILE + "decrypted.txt " + str(key ) + " " + str(thrads_num) +" -e")

        print(HEADER + " Comapre " + ENDC)
        if (filecmp.cmp(PATH_TO_INPUT_FILE + "file_test_1.txt ", PATH_TO_INPUT_FILE + "encrypted.txt")):
            print(OKGREEN + " PASS  - TEST" +   ENDC )
        else:
            print (FAIL + "FALIED" + ENDC)

        print('\n')



# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#                                                   Test 9
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class test_Caesar_9 (test):
    def __init__(self):
        super(test_Caesar_9, self).__init__(9)

    def run (self):
        thrads_num = 60
        key = 1629
        print (BOLD + "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" + ENDC )
        print(OKBLUE + " Test " + str(self.test_number)  + ENDC)
        print(OKBLUE + " [key  = {key}, thrads_num = {thrads_num} ]".format(key = key,thrads_num = thrads_num ) + ENDC)

        print(HEADER + " Loading App step 1"  + ENDC)
        os.system(PATH_TO_APP + PATH_TO_INPUT_FILE + "file_test_1.txt " + str(key ) + " " + str(thrads_num) + " -e")

        print(HEADER + " Loading App step 2"  + ENDC)
        os.system(PATH_TO_APP + PATH_TO_INPUT_FILE + "encrypted.txt " + str(key ) + " " + str(thrads_num) +" -d")

        print(HEADER + " Comapre " + ENDC)
        if (filecmp.cmp(PATH_TO_INPUT_FILE + "file_test_1.txt ", PATH_TO_INPUT_FILE + "decrypted.txt")):
            print(OKGREEN + " PASS  - TEST" +   ENDC )
        else:
            print (FAIL + "FALIED" + ENDC)

        print('\n')



# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#                                                   Test 10
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class test_Caesar_10 (test):
    def __init__(self):
        super(test_Caesar_10, self).__init__(10)

    def run (self):
        thrads_num = 44
        key = -66
        print (BOLD + "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" + ENDC )
        print(OKBLUE + " Test " + str(self.test_number)  + ENDC)
        print(OKBLUE + " [key  = {key}, thrads_num = {thrads_num} ]".format(key = key,thrads_num = thrads_num ) + ENDC)

        print(HEADER + " Loading App step 1"  + ENDC)
        os.system(PATH_TO_APP + PATH_TO_INPUT_FILE + "empty_file.txt " + str(key ) + " " + str(thrads_num) + " -e")

        print(HEADER + " Loading App step 2"  + ENDC)
        os.system(PATH_TO_APP + PATH_TO_INPUT_FILE + "encrypted.txt " + str(key ) + " " + str(thrads_num) +" -d")

        print(HEADER + " Comapre " + ENDC)
        if (filecmp.cmp(PATH_TO_INPUT_FILE + "empty_file.txt ", PATH_TO_INPUT_FILE + "decrypted.txt")):
            print(OKGREEN + " PASS  - TEST" +   ENDC )
        else:
            print (FAIL + "FALIED" + ENDC)

        print('\n')



# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#                                                   Test 5
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class test_Caesar_11 (test):
    def __init__(self):
        super(test_Caesar_11, self).__init__(11)

    def run (self):
        thrads_num = 2
        key = 6
        print (BOLD + "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" + ENDC )
        print(OKBLUE + " Test " + str(self.test_number)  + ENDC)
        print(OKBLUE + " [key  = {key}, thrads_num = {thrads_num} ]".format(key = key,thrads_num = thrads_num ) + ENDC)

        print(HEADER + " Loading App step 1"  + ENDC)
        os.system(PATH_TO_APP + PATH_TO_INPUT_FILE + "plaintext_example.txt " + str(key ) + " " + str(thrads_num) + " -e")

        print(HEADER + " Loading App step 2"  + ENDC)
        os.system(PATH_TO_APP + PATH_TO_INPUT_FILE + "encrypted.txt " + str(key ) + " " + str(thrads_num) +" -d")

        print(HEADER + " Comapre " + ENDC)
        if (filecmp.cmp(PATH_TO_INPUT_FILE + "plaintext_example.txt ", PATH_TO_INPUT_FILE + "decrypted.txt")):
            print(OKGREEN + " PASS  - TEST" +   ENDC )
        else:
            print (FAIL + "FALIED" + ENDC)

        print('\n')

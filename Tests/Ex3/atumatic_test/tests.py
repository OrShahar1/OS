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

PATH_TO_DESKTOP = "C:\Users\User\Desktop\\"
PATH_TO_PROJECTS = "C:\Users\User\Desktop\os\IPS_Projects"
PATH_TO_APP = PATH_TO_PROJECTS+"\IPS_Ex3\Debug\Factori.exe "
PATH_TO_INPUT_FILE = PATH_TO_PROJECTS + "\Tests\Ex3\\"


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#                                                 Test 1
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class test_1 (test):
    def __init__(self):
        super(test_1 ,self).__init__(1)

    def run (self):
        thrads_num = 1
        task_number = 3
        print (BOLD + "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" + ENDC )
        print(OKBLUE + " Test " + str(self.test_number)  + ENDC)
        print(OKBLUE + " [task_number  = {task_number}, thrads_num = {thrads_num} ]".format(task_number = task_number,thrads_num = thrads_num ) + ENDC)

        file = open (PATH_TO_INPUT_FILE + "t_1.txt ", "r")
        lines = file.readlines()[:task_number]
        file = open(PATH_TO_INPUT_FILE + "t_1.txt ", "w")
        for line in lines:
            file.write(line)
        file.close()

        os.system(PATH_TO_APP + PATH_TO_INPUT_FILE + "t_1.txt " + PATH_TO_INPUT_FILE + "pr_1.txt " + str(task_number ) + " " + str(thrads_num))
        os.startfile( PATH_TO_INPUT_FILE + "t_1.txt " )
        os.startfile( PATH_TO_INPUT_FILE + "pr_1.txt " )


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#                                                 Test 2
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class test_2 (test):
    def __init__(self):
        super(test_2 ,self).__init__(2)

    def run (self):
        thrads_num = 1
        task_number = 3
        print (BOLD + "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" + ENDC )
        print(OKBLUE + " Test " + str(self.test_number)  + ENDC)
        print(OKBLUE + " [task_number  = {task_number}, thrads_num = {thrads_num} ]".format(task_number = task_number,thrads_num = thrads_num ) + ENDC)

        file = open (PATH_TO_INPUT_FILE + "t_2.txt ", "r")
        lines = file.readlines()[:task_number]
        file = open(PATH_TO_INPUT_FILE + "t_2.txt ", "w")
        for line in lines:
            file.write(line)
        file.close()

        os.system(PATH_TO_APP + PATH_TO_INPUT_FILE + "t_2.txt " + PATH_TO_INPUT_FILE + "pr_2.txt " + str(task_number ) + " " + str(thrads_num))
        os.startfile( PATH_TO_INPUT_FILE + "t_2.txt " )
        os.startfile( PATH_TO_INPUT_FILE + "pr_2.txt " )



# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#                                                 Test 3
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class test_3 (test):
    def __init__(self):
        super(test_3 ,self).__init__(3)

    def run (self):
        thrads_num = 1
        task_number = 4
        print (BOLD + "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" + ENDC )
        print(OKBLUE + " Test " + str(self.test_number)  + ENDC)
        print(OKBLUE + " [task_number  = {task_number}, thrads_num = {thrads_num} ]".format(task_number = task_number,thrads_num = thrads_num ) + ENDC)

        file = open (PATH_TO_INPUT_FILE + "t_3.txt ", "r")
        lines = file.readlines()[:task_number]
        file = open(PATH_TO_INPUT_FILE + "t_3.txt ", "w")
        for line in lines:
            file.write(line)
        file.close()

        os.system(PATH_TO_APP + PATH_TO_INPUT_FILE + "t_3.txt " + PATH_TO_INPUT_FILE + "pr_3.txt " + str(task_number ) + " " + str(thrads_num))
        os.startfile( PATH_TO_INPUT_FILE + "t_3.txt " )
        os.startfile( PATH_TO_INPUT_FILE + "pr_3.txt " )


#  Orsha

class test (object):
    def __init__(self, test_number):
        self.test_number = test_number

    def run (self):
        raise NotImplementedError("Can't use test without run method")

from threading import Thread
from random import randint
from time import sleep


class Consumer(Thread):
    def __init__ (self, num):
        Thread.__init__(self)
        self.num = num
        self.name = f"Im the {num} thread!"
        self.buffer = [0, 1, 3]

    def take_in_buffer(self):
        value = self.buffer.pop()
        print(self.name)
        sleep(randint(1, 9999999) % 2)
        print(f"i took the product {value}")

    def run(self):
        while(True):
            self.take_in_buffer()


if __name__ == "__main__":
    c = Consumer(2)
    c.start()

from threading import Thread
from time import sleep
from random import randint


class Producer(Thread):

    def __init__ (self, id):
        Thread.__init__(self)
        self.id = id
        self.buffer = []
        self.name = f"Im the {self.id} thread !"

    def insert_in_buffer(self):
        print(self.name)
        product = randint(1, 999999)
        self.buffer.append(product)
        sleep(randint(1,5) % 5)

    def run(self):
        while(True):
            self.insert_in_buffer()
            print(self.buffer)

if __name__ == "__main__":
    p = Producer(2)
    p.start()
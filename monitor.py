from consumer import Consumer
from producer import Producer


class Monitor():
    def __init__(self, buffer_size, qtd_consumer, qtd_producer):
        self.buffer = [-1 for _ in range(buffer_size)]
        self.buffer_size = buffer_size
        self.last_index = 0
        self.producer_flag = True
        self.consumer_flag = True

        self.qtd_producer = qtd_producer
        self.qtd_consumer = qtd_consumer

    def add_buffer(self, value):
        if self.producer_flag:
            self.last_index += 1
            self.buffer[self.last_index] = value
            if self.last_index == self.buffer_size+1:
                self.producer_flag = False
        else:
            print('Buffer is full!')

    def pop_buffer(self):
        if self.consumer_flag:
            value = self.buffer[self.last_index] 
            self.buffer[self.last_index] = -1
            self.last_index -= 1
            if not self.last_index:
                self.consumer_flag = False
        else:
            value = -1

        return value

    def run(self):
        consumers = [Consumer() for _ in rang(self.qtd_consumer)]
        producers = [Producer() for _ in rang(self.qtd_producer)]

        while True:
            pass

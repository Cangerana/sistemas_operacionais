from consumer import Consumer 
def setup():
    pass

def status():
    pass


if __name__ == '__main__':
    count = 0 
    qnt_tasks = 20
    qtd_producer = 5
    qtd_consumer = 5
    buffer_size = 100


    while(count < 10):
        count += 1 
        c = Consumer(count)
        c.start()

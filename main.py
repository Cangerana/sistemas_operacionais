# a classe main apenas inicia o trabalho das threads
# onde é setado as quantidades de tasks de produtores e consumidores etc
import threading
from time import sleep
from random import randint
from consumer import Consumer 
from producer import Producer

empty_slot = -1
buffer_size = 5
# buffer with all values equal -1    
buffer = [-1 for _ in range(buffer_size)]
# semaphore.acquire() semaphore.release()
semaphore = threading.Semaphore()
id_c = 0
id_p = 0


def producer():
    global id_p
    id_p += 1
    id_instance = id_p
    while(True):
        semaphore.acquire()

        if empty_slot == buffer_size - 3:
            print(f'Hi, im producer[{id_instance}] and Im waiting buffer is less than {buffer_size}')
        else:
            print(f'Hi, im producer[{id_instance}]. Was producer: {produce_value()}')

        semaphore.release()
        print("------")
        print(buffer)
        print("------")
        sleep(randint(1, 99999999) % 2)


def produce_value():
    global empty_slot
    empty_slot += 1
    buffer[empty_slot] = randint(1, 10)

    return buffer[empty_slot]


def consumer():
    #consume 
    global id_c
    id_c += 1
    id_instance = id_c
    while(True):
        semaphore.acquire()

        if empty_slot < 0: 
            print(f"Hi, im consumer[{id_instance}]. Im consumer and im waiting")
        else:
            print(f'Hi, im consumer[{id_instance}]. Foi consumido: {consume_value()}')

        semaphore.release()
        print("------")
        print(buffer)
        print("------")
        sleep(randint(1, 99999999) % 2)


def consume_value():
    global empty_slot
    value = buffer[empty_slot]
    buffer[empty_slot] = -1
    empty_slot -= 1

    return value


# safe vou fazer a funcao de consumir 
def main():    
    # inserido pelo [usuario]
    amt_consumer = 80
    amt_producer = 80

    print("Preset finished!")
    print("__________________________________")
    print("Starting ...!")

    for p in range(amt_consumer):
        td = threading.Thread(target=producer, name=str(p))
        td.start()

    for c in range(amt_consumer):
        td = threading.Thread(target=consumer, name=str(c))
        td.start()


if __name__ == '__main__':
    main()

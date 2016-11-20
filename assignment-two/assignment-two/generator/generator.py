from random import randint

NUM_COMMANDS = 30000
MAX_LORRY_ID = 5
MAX_STRENGTH = 10
MAX_ANIMAL_ID = 20
ADD_CHANCE = 3 # chance to generate add command, minimum is 3, 3 means 33%, higher value means higher chance

for i in range(NUM_COMMANDS):
    cmd = randint(1, ADD_CHANCE)
    if cmd > 2:
        print("{cmd} {lorry} {id} {pwr}"
              .format(cmd=0, lorry=randint(0,MAX_LORRY_ID), id=randint(0,MAX_ANIMAL_ID), pwr=randint(0,MAX_STRENGTH)))
    elif cmd == 1:
        print("{cmd} {lorry}".format(cmd=1, lorry=randint(0, MAX_LORRY_ID)))
    elif cmd == 2:
        print("{cmd} {lorry}".format(cmd=2, lorry=randint(0, MAX_LORRY_ID)))
print(4)

import itertools, random

from computer_move import computer_move

from functions import draw_hand
from functions import should_game_end
from functions import print_values


from player_options import Play_all
from player_options import Play_card
from player_options import Buy_card
from player_options import End_turn

from game_setup import setup

class Card(object):
    def __init__(self, name, values=(0, 0), cost=1, clan=None):
        self.name = name
        self.cost = cost
        self.values = values
        self.clan = clan
    def __str__(self):
                return 'Name %s costing %s with attack %s and money %s' % (self.name, self.cost, self.values[0], self.values[1])
    def get_attack(self):
        return self.values[0]
    def get_money(self):
            return self.values[1]


if __name__ == '__main__':
    Player = {'name': 'player one', 'health': 30, 'deck': None, 'hand': None, 'active': None, 'handsize': 5,
                 'discard': None}
    Computer = {'name': 'player computer', 'health': 30, 'deck': None, 'hand': None, 'active': None, 'handsize': 5,
                            'discard': None}
    central_line = {'name': 'central_line', 'active': None, 'activeSize': 5, 'supplement': None, 'deck': None}

    setup(Card, central_line, Player, Computer, itertools, random)




    play_game = raw_input('Do you want to play a game?:\t')
    playing_game = (play_game=='Y')

    oT = raw_input("Do you want an aggressive (A) opponent or an acquisative (Q) opponent?\t")
    aggressive = (oT=='A')


    while playing_game:
	money = 0
        attack = 0
	draw_hand(Player, random)
        while True:

	    ### Player's Turn Begins ###
            print_values(Player, Computer, money, attack)

            action = raw_input("Enter Action: ")
            print action

            if action == 'P':
                money, attack = Play_all(Player, money, attack)

            if action.isdigit():
                money, attack = Play_card(action, Player, money, attack)

            if (action == 'B'):
                money = Buy_card(Player, central_line, money)

            if action == 'A':
                Computer['health'] = Computer['health'] - attack
                attack = 0

            if action == 'E':
                End_turn(Player, random)
                break
	    ### Player's Turn End ###

	### Computer's Turn Begins ###
	draw_hand(Computer, random)
        computer_move(central_line, Player, Computer, aggressive, random)
	### Computer's Turn End ###

	playing_game = should_game_end(Player, Computer, central_line, random, playing_game)

	if not playing_game:
            play_game = raw_input("\nDo you want to play another game?:")
            playing_game = (play_game=='Y')
            if playing_game:
                oT = raw_input("Do you want an aggressive (A) opponent or an acquisative (Q) opponent")
                aggressive = (oT=='A')

                setup(Card, central_line, Player, Computer, itertools, random)


    exit()

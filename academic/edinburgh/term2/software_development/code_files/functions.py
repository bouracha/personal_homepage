
def draw_hand(Player, random):
    for x in range(0, Player['handsize']):
        if (len(Player['deck']) == 0):
            random.shuffle(Player['discard'])
            Player['deck'] = Player['discard']
            Player['discard'] = []
        card = Player['deck'].pop()
        Player['hand'].append(card)

def should_game_end(Player, Computer, central_line, random, playing_game):
	if Player['health'] <= 0:
        	playing_game = False
        	print "Computer wins"
        elif Computer['health'] <= 0:
        	playing_game = False
        	print 'Player One Wins'
        elif central_line['activeSize'] == 0:
        	print "No more cards available"
        	if Player['health'] > Computer['health']:
                	print "Player One Wins on Health"
                elif Computer['health'] > Player['health']:
                	print "Computer Wins on health"
                else:
                	player_card_stregnth = 0
                	computer_card_stregnth = 0
                	while True:
                        	if (len(Player['deck']) == 0):
                        		random.shuffle(Player['discard'])
                        		Player['deck'] = Player['discard']
                        		Player['discard'] = []
                    		card = Player['deck'].pop()
                    		player_card_stregnth += card.get_attack()
                    		computer_card_stregnth += card.get_attack()
		    		if (player_card_stregnth > Computer['health']):
		        		print "Player Wins on card strength!"
                        		break
		    		if (computer_card_stregnth > Player['health']):
		        		print "Computer Wins on card strength!"
                       		break
		playing_game = False
	return playing_game


def print_values(Player, Computer, money, attack):
	print "\n########################################################################"
        print "########################################################################"

        print "\nPlayer Health %s" % Player['health']
        print "Computer Health %s" % Computer['health']

        print "\nYour Hand"
        index = 0
        for card in Player['hand']:
	        print "[%s] %s" % (index, card)
                index = index + 1
        print "\nYour Values"
        print "Money %s, Attack %s" % (money, attack)
        print "\nChoose Action: (P = play all, [0-n] = play that card, B = Buy Card, A = Attack, E = end turn)"

        print "\n########################################################################"
        print "########################################################################\n"

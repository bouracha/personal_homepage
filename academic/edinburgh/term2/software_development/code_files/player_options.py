from functions import draw_hand

def Play_all(Player, money, attack):
	if(len(Player['hand'])>0):
		for x in range(0, len(Player['hand'])):
                	card = Player['hand'].pop()
                        Player['active'].append(card)
                        money = money + card.get_money()
                        attack = attack + card.get_attack()
	print "\nYour Hand"
	index = 0
	for card in Player['hand']:
		print "[%s] %s" % (index, card)
		index = index + 1
	print "\nYour Active Cards"
	for card in Player['active']:
		print card
	print "\nYour Values"
	print "Money %s, Attack %s" % (money, attack)

	return money, attack

def Play_card(action, Player, money, attack):
	if( int(action) < len(Player['hand'])):
		card = Player['hand'].pop(int(action))
	        Player['active'].append(card)
	        money = money + card.get_money()
                attack = attack + card.get_attack()
	print "\nYour Hand"
        index = 0
        for card in Player['hand']:
	        print "[%s] %s" % (index, card)
                index = index + 1

	print "\nYour Active Cards"
        for card in Player['active']:
	        print card
        print "\nYour Values"
	print "Money %s, Attack %s" % (money, attack)

	return money, attack



def Buy_card(Player, central_line, money):
	notending = True
	while money > 0:
		print "Available Cards"
                ind = 0
                for card in central_line['active']:
                	print "[%s] %s" % (ind, card)
                        ind = ind + 1

                print "Supplement"
                if len(central_line['supplement']) > 0:
                	print central_line['supplement'][0]

                print "\n########################################################################\n"

                print "Choose a card to buy [0-n], S for supplement, E to end buying"
                buy_action = raw_input("Choose option: ")
                if buy_action == 'S':
                	if len(central_line['supplement']) > 0:
                        	if money >= central_line['supplement'][0].cost:
                                	money = money - central_line['supplement'][0].cost
                                	Player['discard'].append(central_line['supplement'].pop())
                                	print "Supplement Bought"
                            	else:
                                	print "insufficient money to buy"
                        else:
                        	print "no supplements left"
		elif buy_action == 'E':
			notending = False
                        break;
		elif buy_action.isdigit():
	                if int(buy_action) < len(central_line['active']):
	                        if money >= central_line['active'][int(buy_action)].cost:
	                                money = money - central_line['active'][int(buy_action)].cost
	                                Player['discard'].append(central_line['active'].pop(int(buy_action)))
	                                if( len(central_line['deck']) > 0):
						card = central_line['deck'].pop()
						central_line['active'].append(card)
	                                else:
		                                central_line['activeSize'] = central_line['activeSize'] - 1
        	                        print "Card bought"
				else:
	                                print "insufficient money to buy"
			else:
	                        print "enter a valid index number"
		else:
		        print "Enter a valid option"

	return money



def End_turn(Player, random):
	if (len(Player['hand']) >0 ):
		for x in range(0, len(Player['hand'])):
        		Player['discard'].append(Player['hand'].pop())

	if (len(Player['active']) >0 ):
		for x in range(0, len(Player['active'])):
        		Player['discard'].append(Player['active'].pop())



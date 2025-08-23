from player_options import End_turn

def computer_move(central_line, Player, Computer, aggressive, random):
    print "Available Cards"
    for card in central_line['active']:
        print card

    print "Supplement"
    if len(central_line['supplement']) > 0:
        print central_line['supplement'][0]

    print "\nPlayer Health %s" % Player['health']
    print "Computer Health %s" % Computer['health']

    money = 0
    attack = 0
    for x in range(0, len(Computer['hand'])):
                    card = Computer['hand'].pop()
                    Computer['active'].append(card)
                    money = money + card.get_money()
                    attack = attack + card.get_attack()

    print " Computer player values attack %s, money %s" % (attack, money)
    print " Computer attacking with strength %s" % attack
    Player['health'] = Player['health'] - attack
    attack = 0
    print "\nPlayer Health %s" % Player['health']
    print "Computer Health %s" % Computer['health']
    print " Computer player values attack %s, money %s" % (attack, money)
    print "Computer buying"
    if money > 0:
        computerbuy = True
        templist = []
        print "Starting Money %s and computerbuy %s " % (money, computerbuy)
        while computerbuy:
            templist = []
            if len(central_line['supplement']) > 0:
                if central_line['supplement'][0].cost <= money:
                    templist.append(("S", central_line['supplement'][0]))
            for intindex in range (0, central_line['activeSize']):
                if central_line['active'][intindex].cost <= money:
                    templist.append((intindex, central_line['active'][intindex]))
            if len(templist) > 0:
                highestIndex = 0
                for intindex in range(0,len(templist)):
                    if templist[intindex][1].cost > templist[highestIndex][1].cost:
                        highestIndex = intindex
                    if templist[intindex][1].cost == templist[highestIndex][1].cost:
                        if aggressive:
                            if templist[intindex][1].get_attack() >templist[highestIndex][1].get_attack():
                                highestIndex = intindex
                        else:
                            if templist[intindex][1].get_attack() >templist[highestIndex][1].get_money():
                                highestIndex = intindex
                source = templist[highestIndex][0]
                if source in range(0,5):
                    if money >= central_line['active'][int(source)].cost:
                        money = money - central_line['active'][int(source)].cost
                        card = central_line['active'].pop(int(source))
                        print "Card bought %s" % card
                        Computer['discard'].append(card)
                        if( len(central_line['deck']) > 0):
                            card = central_line['deck'].pop()
                            central_line['active'].append(card)
                        else:
                            central_line['activeSize'] = central_line['activeSize'] - 1
                    else:
                        print "Error Occurred"
                else:
                    if money >= central_line['supplement'][0].cost:
                        money = money - central_line['supplement'][0].cost
                        card = central_line['supplement'].pop()
                        Computer['discard'].append(card)
                        print "Supplement Bought %s" % card
                    else:
                        print "Error Occurred"
            else:
                computerbuy = False
            if money == 0:
                computerbuy = False
    else:
        print "No Money to buy anything"

    End_turn(Computer, random)

    print "Computer turn ending"

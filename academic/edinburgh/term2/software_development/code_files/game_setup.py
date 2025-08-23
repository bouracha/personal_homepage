    
def setup(Card, central_line, Player, Computer, itertools, random):
    sdc = [
                                4 * [Card('Archer', (3, 0), 2)],
                                4 * [Card('Baker', (0, 3), 2)],
                                3 * [Card('Swordsman', (4, 0), 3)],
                                2 * [Card('Knight', (6, 0), 5)],
                                3 * [Card('Tailor', (0, 4), 3)],
                                3 * [Card('Crossbowman', (4, 0), 3)],
                                3 * [Card('Merchant', (0, 5), 4)],
                                4 * [Card('Thug', (2, 0), 1)],
                                4 * [Card('Thief', (1, 1), 1)],
                                2 * [Card('Catapault', (7, 0), 6)],
                                2 * [Card('Caravan', (1, 5), 5)],
                                2 * [Card('Assassin', (5, 0), 4)]
          ]
    playeronedeck = [8 * [Card('Serf', (0, 1), 0)],
                     2 * [Card('Squire', (1, 0), 0)]
                    ]
    pod = list(itertools.chain.from_iterable(playeronedeck))
    Player['deck'] = pod
    Player['hand'] = []
    Player['discard'] = []
    Player['active'] = []
    playertwodeck = [
            8 * [Card('Serf', (0, 1), 0)],
        2 * [Card('Squire', (1, 0), 0)]
    ]
    ptd = list(itertools.chain.from_iterable(playertwodeck))
    Computer['deck'] = ptd
    Computer['hand'] = []
    Computer['discard'] = []
    Computer['active'] = []

    supplement = 10 * [Card('Levy', (1, 2), 2)]
    deck = list(itertools.chain.from_iterable(sdc))
    random.shuffle(deck)
    central_line['deck'] = deck
    central_line['supplement'] = supplement
    central_line['active'] = []

    Player['health'] = 30
    Computer['health'] = 30

    for x in range(0, central_line['activeSize']):
        card = central_line['deck'].pop()
        central_line['active'].append(card)

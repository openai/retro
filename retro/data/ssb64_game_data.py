import collections

from retro.data.ssb64_ram import SSB64RAM


class SSB64GameData:
    # Assume only two players.
    num_players = 2

    def __init__(self):
        self.ram = None
        self.player_state = None
        self.reset()

    def reset(self):
        self.ram = SSB64RAM()
        self.player_state = collections.defaultdict(lambda: collections.defaultdict(int))

    def update(self, ram):
        self.ram.update(ram)
        for player_index in range(self.num_players):
            stock = self.ram.player_stock(player_index)
            # Stock should only decrease (handles an edge case with defaultdict usage / initialization).
            stock_change = min(0, stock - self.player_state[player_index]["stock"])
            self.player_state[player_index]["stock"] = stock
            self.player_state[player_index]["stock_change"] = stock_change

            damage = self.ram.player_damage(player_index)
            damage_change = damage - self.player_state[player_index]["damage"]
            self.player_state[player_index]["damage"] = damage
            if stock_change == 0:
                self.player_state[player_index]["damage_change"] = damage_change
            else:
                # When a stock is lost, don't count the change in damage.
                self.player_state[player_index]["damage_change"] = 0

    def current_reward(self, player_index=0):
        stock_change = self.player_state[player_index]["stock_change"]
        if stock_change != 0:
            return -100

        # Penalize taking damage.
        reward = -self.player_state[player_index]["damage_change"]

        # Reward dealing damage or having other players lose a stock.
        for other_player_index in range(self.num_players):
            if other_player_index == player_index:
                continue
            reward += self.player_state[other_player_index]["damage_change"]
            if self.player_state[other_player_index]["stock_change"] != 0:
                reward += 100

        return reward

    def is_done(self):
        nonzero_stock_count = 0
        for player_index in range(self.num_players):
            if self.player_state[player_index]["stock"] > 0:
                nonzero_stock_count += 1
        return nonzero_stock_count == 1

    def lookup_all(self):
        return dict()

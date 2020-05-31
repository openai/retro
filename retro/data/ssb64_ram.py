"""This file contains a class that interprets the ram for SSB64.

Most of this information is from:
# https://github.com/Isotarge/ScriptHawk/blob/master/games/smash64.lua
"""
import numpy as np

SSB64_CHARACTER_ORDERING = [
    "Mario",
    "Fox",
    "DK",
    "Samus",
    "Luigi",
    "Link",
    "Yoshi",
    "Falcon",
    "Kirby",
    "Pikachu",
    "Jigglypuff",
    "Ness",
]
SSB64_MAP_ORDERING = [
    "Peach's Castle",
    "Sector Z",
    "Kongo Jungle",
    "Planet Zebes",
    "Hyrule Castle",
    "Yoshi's Island",
    "Dream Land",
    "Saffron City",
    "Mushroom Kingdom",
]


# I think this is read <u# (# byte unsigned little endian).
def convert_byte_list_to_int(byte_list):
    return int("".join(reversed([f"{byte:02x}" for byte in byte_list])), 16)


class SSB64RAM:
    """Interprets the SSB64 ram."""

    rambase = 0x80000000
    addr_size = 4

    player_list_ptr = 0x130D84
    player_size = 0xB50
    character_offset = 0x08

    match_settings_ptr = 0xA50E8
    match_settings_size = 0x1c8
    map_offset = 0x02
    players_bases = [0x20, 0x94, 0x108, 0x17C]
    damage_suboffset = 0x4C
    stock_suboffset = 0x8

    def __init__(self):
        self.ram = None

    def update(self, ram):
        self.ram = ram

    def assert_valid_ram_address(self, addr):
        assert addr > 0, f"Address must be > 0: {addr}"
        assert addr < len(self.ram), f"Address must be < size of ram: {addr}"

    def read_address(self, ptr):
        self.assert_valid_ram_address(ptr)
        addr_list = self.ram[ptr:ptr + self.addr_size]
        abs_addr = convert_byte_list_to_int(addr_list)
        rel_addr = abs_addr - self.rambase
        self.assert_valid_ram_address(rel_addr)
        return rel_addr

    @property
    def match_settings(self):
        match_settings_addr = self.read_address(self.match_settings_ptr)
        match_settings = self.ram[match_settings_addr:match_settings_addr +
                                  self.match_settings_size]
        return match_settings

    @property
    def match_map(self):
        match_map_index = self.match_settings[self.map_offset]
        assert match_map_index >= 0 and match_map_index < len(SSB64_MAP_ORDERING)
        return SSB64_MAP_ORDERING[match_map_index]

    def _assert_valid_player_index(self, player_index):
        assert player_index >= 0 and player_index <= 3

    def player_damage(self, player_index):
        self._assert_valid_player_index(player_index)
        dmg_index = self.players_bases[player_index] + self.damage_suboffset
        # Damage is stored as four bytes.
        dmg_bytes = self.match_settings[dmg_index:dmg_index + 4]
        dmg = convert_byte_list_to_int(dmg_bytes)
        return dmg

    def player_stock(self, player_index):
        self._assert_valid_player_index(player_index)
        # Stock is stored starting from 0 so add 1 to give the actual value.
        stock = self.match_settings[self.players_bases[player_index] + self.stock_suboffset] + 1
        # Stock is unsigned, so when it wraps around to 255 + 1 = 256.
        # When this happens return a stock value of 0.
        if stock == 256:
            stock = 0
        assert stock >= 0
        return stock

    def player_data(self, player_index):
        self._assert_valid_player_index(player_index)
        players_addr = self.read_address(self.player_list_ptr)
        start = players_addr + self.player_size * player_index
        end = start + self.player_size
        player = self.ram[start:end]
        return player

    def player_character(self, player_index):
        self._assert_valid_player_index(player_index)
        player_data = self.player_data(player_index)
        character_index = player_data[self.character_offset]
        assert character_index >= 0 and character_index < len(SSB64_CHARACTER_ORDERING)
        return SSB64_CHARACTER_ORDERING[character_index]


def main():
    # filepath = "/home/wulfebw/programming/retro/save_states/dreamland.npy"
    # filepath = "/home/wulfebw/programming/retro/save_states/peaches.npy"
    # filepath = "/home/wulfebw/programming/retro/save_states/kongo.npy"
    # filepath = "/home/wulfebw/programming/retro/save_states/sector_z.npy"
    # filepath = "/home/wulfebw/programming/retro/save_states/4_player.npy"
    # filepath = "/home/wulfebw/programming/retro/save_states/dmg_lives.npy"
    filepath = "/home/wulfebw/programming/retro/save_states/dmg_lives_2.npy"
    ram = np.load(filepath)
    ssb64ram = SSB64RAM()
    ssb64ram.update(ram)


if __name__ == "__main__":
    main()

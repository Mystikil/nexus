import xml.etree.ElementTree as ET
import json
import os

# Load field options from file
with open("monster_field_options.json", "r") as f:
    OPTIONS = json.load(f)

def menu_select(prompt, options, allow_custom=True, multi=False):
    """CLI menu-based selector for options."""
    selected = []
    while True:
        print(f"\n{prompt}")
        for i, opt in enumerate(options, 1):
            print(f"  {i}. {opt}")
        if allow_custom:
            print("  0. Enter custom value")
        choice = input("Choose number (or press enter to finish if multi): ").strip()

        if choice == "" and multi:
            break
        if choice == "0" and allow_custom:
            val = input("  Enter custom value: ")
            if val: selected.append(val)
        elif choice.isdigit() and 1 <= int(choice) <= len(options):
            selected.append(options[int(choice)-1])
        else:
            print("Invalid choice.")
        if not multi:
            break
    return selected if multi else (selected[0] if selected else "")

def prompt_attrs(attr_list):
    result = {}
    for attr in attr_list:
        val = input(f"  {attr} (optional): ").strip()
        if val:
            result[attr] = val
    return result

def get_flags():
    print("\n-- Flags Setup --")
    return {flag: input(f"{flag} (default 0): ").strip() or "0" for flag in OPTIONS["flags"]}

def get_attacks():
    attacks = []
    print("\n-- Attacks --")
    while input("Add an attack? (y/n): ").lower() == "y":
        atk = prompt_attrs(["name", "interval", "chance", "range", "radius", "target", "min", "max", "poison", "duration", "length", "spread", "speedchange"])
        atk_attrs = {}
        while input("  Add attribute to this attack? (y/n): ").lower() == "y":
            key = menu_select("Choose attack attribute:", OPTIONS["attack_attributes"])
            val = input(f"    Value for {key}: ")
            atk_attrs[key] = val
        attacks.append((atk, atk_attrs))
    return attacks

def get_defenses():
    defenses = []
    armor = input("Armor: ")
    defense = input("Defense: ")
    print("\n-- Defenses --")
    while input("Add a special defense? (y/n): ").lower() == "y":
        d = prompt_attrs(["name", "interval", "chance", "min", "max", "duration", "speedchange"])
        attrs = {}
        while input("  Add attribute to defense? (y/n): ").lower() == "y":
            key = menu_select("Choose defense attribute:", OPTIONS["defense_attributes"])
            val = input(f"    Value for {key}: ")
            attrs[key] = val
        defenses.append((d, attrs))
    return armor, defense, defenses

def get_elements():
    print("\n-- Elemental Resistances --")
    elements = []
    while input("Add an element? (y/n): ").lower() == "y":
        key = menu_select("Choose element type:", OPTIONS["element_types"])
        val = input(f"  Value for {key} (%): ")
        elements.append((key, val))
    return elements

def get_immunities():
    print("\n-- Immunities --")
    return menu_select("Select immunities:", OPTIONS["immunities"], multi=True)

def get_loot():
    print("\n-- Loot --")
    loot = []
    while input("Add loot item? (y/n): ").lower() == "y":
        name_or_id = input("  Item name or ID: ")
        chance = input("  Drop chance: ")
        countmax = input("  Max count (optional): ")
        loot.append((name_or_id, chance, countmax))
    return loot

def get_voices():
    print("\n-- Voices --")
    voices = []
    while input("Add a voice line? (y/n): ").lower() == "y":
        sentence = input("  Sentence: ")
        yell = input("  Yell? (y/n): ").lower() == "y"
        voices.append((sentence, yell))
    return voices

def get_summons():
    print("\n-- Summons --")
    max_summons = input("Max summons: ")
    summons = []
    while input("  Add summon? (y/n): ").lower() == "y":
        s = prompt_attrs(["name", "interval", "chance", "max", "speed"])
        summons.append(s)
    return max_summons, summons

def build_monster():
    monster = ET.Element("monster", {
        "name": input("Monster name: "),
        "nameDescription": input("Name description: "),
        "race": input("Race: "),
        "experience": input("Experience: "),
        "speed": input("Speed: ")
    })

    ET.SubElement(monster, "health", {
        "now": input("Health now: "),
        "max": input("Health max: ")
    })

    look = prompt_attrs(["type", "head", "body", "legs", "feet", "corpse", "addons"])
    ET.SubElement(monster, "look", {k: v for k, v in look.items() if v})

    ET.SubElement(monster, "targetchange", {
        "interval": input("Target change interval: "),
        "chance": input("Target change chance: ")
    })

    flags = ET.SubElement(monster, "flags")
    for key, value in get_flags().items():
        ET.SubElement(flags, "flag", {key: value})

    attacks_elem = ET.SubElement(monster, "attacks")
    for atk, attrs in get_attacks():
        atk_elem = ET.SubElement(attacks_elem, "attack", {k: v for k, v in atk.items() if v})
        for k, v in attrs.items():
            ET.SubElement(atk_elem, "attribute", {"key": k, "value": v})

    armor, defense, defenses = get_defenses()
    def_elem = ET.SubElement(monster, "defenses", {"armor": armor, "defense": defense})
    for d, attrs in defenses:
        sub = ET.SubElement(def_elem, "defense", {k: v for k, v in d.items() if v})
        for k, v in attrs.items():
            ET.SubElement(sub, "attribute", {"key": k, "value": v})

    if elements := get_elements():
        elems = ET.SubElement(monster, "elements")
        for k, v in elements:
            ET.SubElement(elems, "element", {k: v})

    if immunities := get_immunities():
        imms = ET.SubElement(monster, "immunities")
        for name in immunities:
            ET.SubElement(imms, "immunity", {name: "1"})

    if input("Add summons? (y/n): ").lower() == "y":
        max_summons, summons = get_summons()
        sum_elem = ET.SubElement(monster, "summons", {"maxSummons": max_summons})
        for s in summons:
            ET.SubElement(sum_elem, "summon", {k: v for k, v in s.items() if v})

    if voices := get_voices():
        voice_elem = ET.SubElement(monster, "voices", {"interval": "5000", "chance": "10"})
        for sentence, yell in voices:
            ET.SubElement(voice_elem, "voice", {
                "sentence": sentence,
                **({"yell": "1"} if yell else {})
            })

    if loot := get_loot():
        loot_elem = ET.SubElement(monster, "loot")
        for name_or_id, chance, countmax in loot:
            attr = {"chance": chance}
            if name_or_id.isdigit():
                attr["id"] = name_or_id
            else:
                attr["name"] = name_or_id
            if countmax:
                attr["countmax"] = countmax
            ET.SubElement(loot_elem, "item", attr)

    # Save XML
    tree = ET.ElementTree(monster)
    ET.indent(tree, space="  ", level=0)
    output_file = input("Save as (filename.xml): ").strip() or "output_monster.xml"
    tree.write(output_file, encoding="utf-8", xml_declaration=True)
    print(f"\n✅ Monster saved to {output_file}")

if __name__ == "__main__":
    build_monster()
# This script is designed to create a monster XML file for a game.
# It prompts the user for various attributes and options, allowing for a flexible monster creation process.
# The generated XML can be used in game development or modding contexts.
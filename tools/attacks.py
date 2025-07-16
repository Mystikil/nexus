import json

# Load categorized attack types
with open("categorized_attacks.json", "r") as f:
    categories = json.load(f)

def show_menu():
    print("\nChoose an attack type category to explore:\n")
    for i, key in enumerate(categories.keys(), 1):
        print(f"{i}. {key}")
    print("0. Exit")

def main():
    while True:
        show_menu()
        choice = input("\nEnter number: ").strip()
        if choice == "0":
            break
        if not choice.isdigit() or int(choice) < 1 or int(choice) > len(categories):
            print("Invalid choice.")
            continue
        category = list(categories.keys())[int(choice)-1]
        print(f"\n🧪 Attacks under '{category}':\n")
        for name in sorted(categories[category]):
            print(f"- {name}")
        input("\nPress Enter to go back to the menu...")

if __name__ == "__main__":
    main()

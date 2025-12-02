from typing import TypedDict, List

class Student(TypedDict):
	jmeno: str
	znamky: List[int]

seznam: List[Student] = []
def pridej_studenta(jmeno: str, znamky: List[int]) -> None:
	student: Student = {"jmeno": jmeno, "znamky": znamky}
	seznam.append(student)


def _prumer_znamek(znamky: List[int]) -> float:
	return sum(znamky) / len(znamky)


def nejlepsi_student(seznam_studentu: List[Student]) -> Student:
	best = min(seznam_studentu, key=lambda s: _prumer_znamek(s.get("znamky", [])))
	prumer = _prumer_znamek(best.get("znamky", []))
	print(f"Nejlepší student: {best['jmeno']} (průměr {prumer:.2f})")
	return best


def _parse_znamky(text: str) -> List[int]:
	if not text:
		return []
	parts = [p.strip() for p in text.replace(',', ' ').split()]
	if not parts:
		return []
	znamky: List[int] = []
	for p in parts:
		if p == '':
			continue
		try:
			z = int(p)
		except ValueError:
			raise ValueError(f"Neplatná známka: {p}")
		znamky.append(z)
	return znamky


if __name__ == "__main__":
	try:
		while True:
			try:
				raw = input("Kolik studentů chcete zadat? (min. 3) ")
				pocet = int(raw)
				if pocet < 3:
					print("Zadejte číslo alespoň 3.")
					continue
				break
			except ValueError:
				print("Zadejte platné celé číslo.")

		for i in range(1, pocet + 1):
			while True:
				jmeno = input(f"Jméno studenta {i}: ").strip()
				if jmeno:
					break
				print("Jméno nesmí být prázdné.")

			while True:
				raw_z = input("Zadejte známky oddělené čárkou (např. 1,2,3): ").strip()
				try:
					znamky = _parse_znamky(raw_z)
					if not znamky:
						print("Musíte zadat alespoň jednu známku.")
						continue
					break
				except ValueError as e:
					print(e)

			pridej_studenta(jmeno, znamky)

		print("\nSeznam studentů byl vytvořen.")
		nejlepsi_student(seznam)
	except KeyboardInterrupt:
		print("\nZrušeno uživatelem.")

#include <iostream>
#include <vector>
#include <stdexcept>
using namespace std;

class EmptyContainerException : public runtime_error {
public:
	EmptyContainerException() : runtime_error("Error, Vector is empty") {}
};


void Find(const vector<int>& numbers) {
	if (numbers.empty()) {
		throw EmptyContainerException();
	}

	auto min_it = numbers.begin();
	auto max_it = numbers.begin();

	for (auto it = numbers.begin(); it != numbers.end(); ++it) {
		if (*it < *min_it) min_it = it;
		if (*it > *max_it) max_it = it;
	}

	cout << "Min val: " << *min_it << endl;
	cout << "Max val: " << *max_it << endl;
}

int main() {
	vector<int> numbers;
	int value;

	cout << "Enter values ";
	cout << "Ctrl D щоб оперції були";
	while (cin >> value) {
		numbers.push_back(value);
	}

	try {
		Find(numbers);
	}
	catch (const EmptyContainerException& e) {
		cerr << e.what() << endl;
	}

	return 0;
}
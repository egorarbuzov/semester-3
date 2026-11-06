using System;

public class PredicateComposer<T>
{
    private Predicate<T> _rule;

    public PredicateComposer() : this(x => true) { }

    public PredicateComposer(Predicate<T> rule)
    {
        _rule = rule;
    }

    public PredicateComposer<T> And(Predicate<T> other)
    {
        return new PredicateComposer<T>(x => _rule(x) && other(x));
    }

    public PredicateComposer<T> Or(Predicate<T> other)
    {
        return new PredicateComposer<T>(x => _rule(x) || other(x));
    }

    public PredicateComposer<T> Not()
    {
        return new PredicateComposer<T>(x => !_rule(x));
    }

    public Predicate<T> Build()
    {
        return _rule;
    }
}

class Program
{
    static void PrintFiltered(string title, int[] source, Predicate<int> rule)
    {
        int[] result = Array.FindAll(source, rule);
        Console.WriteLine($"{title}: [{string.Join(", ", result)}]");
    }

    static void Main()
    {
        int[] numbers = { -10, -3, 0, 2, 5, 7, 12, 20, -1, 9 };

        Predicate<int> rule1 = new PredicateComposer<int>()
            .And(x => x > 5)
            .Or(x => x < 0)
            .Not()
            .Build();

        PrintFiltered("NOT (x > 5 OR x < 0)", numbers, rule1);

        Predicate<int> rule2 = new PredicateComposer<int>()
            .And(x => x > 0)
            .And(x => x % 2 == 0)
            .Build();

        PrintFiltered("x > 0 AND чётное", numbers, rule2);

        string[] words = { "яблоко", "Дом", "Анна", "стол", "Кот", "Окно", "мир" };

        Predicate<string> longOrTitle = new PredicateComposer<string>()
            .And(s => s.Length > 4)
            .Or(s => char.IsUpper(s[0]))
            .Build();

        string[] filteredWords = Array.FindAll(words, longOrTitle);
        Console.WriteLine("Строки (длина > 4 OR заглавная): [" + string.Join(", ", filteredWords) + "]");
    }
}
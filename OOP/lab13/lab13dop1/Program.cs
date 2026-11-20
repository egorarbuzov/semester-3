
class Program
{
    static void Main(string[] args)
    {
        string text = "Коллекции в C# позволяют хранить группы объектов. " +
                      "Коллекции делятся на обобщенные и необобщенные. " +
                      "Список предоставляет доступ по индексу, словарь хранит пары ключ значение, " +
                      "очередь работает по принципу первым пришел первым ушел, " +
                      "стек работает по принципу последним пришел первым ушел. " +
                      "Множество хранит только уникальные элементы. " +
                      "Каждая коллекция имеет свои преимущества и недостатки.";

        char[] separators = new char[] { ' ', '.', ',', '!', '?', ';', ':', '-', '\n', '\r', '\t' };
        string[] rawWords = text.Split(separators, StringSplitOptions.RemoveEmptyEntries);

        List<string> words = new List<string>();
        foreach (string word in rawWords)
        {
            words.Add(word.ToLower());
        }

        if (words.Count == 0)
        {
            Console.WriteLine("Текст не содержит слов");
            return;
        }

        Dictionary<string, int> frequency = new Dictionary<string, int>();
        foreach (string word in words)
        {
            if (frequency.ContainsKey(word))
            {
                frequency[word]++;
            }
            else
            {
                frequency.Add(word, 1);
            }
        }

        HashSet<string> uniqueWords = new HashSet<string>();
        foreach (string word in words)
        {
            uniqueWords.Add(word);
        }

        SortedDictionary<string, int> sortedFrequency = new SortedDictionary<string, int>();
        foreach (KeyValuePair<string, int> pair in frequency)
        {
            sortedFrequency.Add(pair.Key, pair.Value);
        }

        Queue<string> history = new Queue<string>();
        foreach (string word in words)
        {
            history.Enqueue(word);
            if (history.Count > 10)
            {
                history.Dequeue();
            }
        }

        List<KeyValuePair<string, int>> top = new List<KeyValuePair<string, int>>(frequency);
        top.Sort((a, b) => b.Value.CompareTo(a.Value));

        Console.WriteLine($"Общее количество слов: {words.Count}");
        Console.WriteLine($"Количество уникальных слов: {uniqueWords.Count}");

        Console.WriteLine("Топ-10 самых частотных слов: ");
        int limit = top.Count < 10 ? top.Count : 10;
        for (int i = 0; i < limit; i++)
        {
            Console.WriteLine($"  {i + 1}. {top[i].Key}: {top[i].Value}");
        }
    }
}

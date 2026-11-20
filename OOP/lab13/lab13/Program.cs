class Program
{
    static void Main(string[] args)
    {
        List<int> list = new List<int>();
        Random rnd = new Random();
        for (int i = 0; i < 20; i++)
        {
            list.Add(rnd.Next(1, 100));
        }

        Console.WriteLine("Исходный список: ");
        foreach (int item in list)
        {
            Console.Write($"{item} ");
        }

        Console.WriteLine();
        
        int even_counter = 0;
        int even_sum = 0;
        foreach (int item in list)
        {
            if (item % 2 == 0)
            {
                Console.Write($"{item} ");
                even_counter++;
                even_sum += item;
            }   
        }
        list.RemoveAll(x => x % 2 != 0);

        Console.WriteLine("\nПолученный список: ");
        foreach (int item in list)
        {
            Console.Write($"{item} ");
        }

        Console.WriteLine();

        Console.WriteLine($"Количество четных: {even_counter}");
        Console.WriteLine($"Сумма четных: {even_sum}");
        
        Dictionary<string, int> dict = new Dictionary<string, int>();
        dict.Add("Книга", 30);
        dict.Add("Ноутбук", 120);
        dict.Add("Смартфон", 105);
        dict.Add("Ручка", 5);
        dict.Add("Лампочка", 15);

        string bestItem = null;
        int maxCost = int.MinValue;

        foreach (KeyValuePair<string, int> item in dict)
        {
            if (item.Value > maxCost)
            {
                maxCost = item.Value;
                bestItem = item.Key;
            }
        }

        Console.WriteLine($"Самый дорогой предмет: {bestItem} — {maxCost}");
        
        string worstItem = null;
        int minCost = int.MaxValue;

        foreach (KeyValuePair<string, int> item in dict)
        {
            if (item.Value < minCost)
            {
                minCost = item.Value;
                worstItem = item.Key;
            }
        }

        Console.WriteLine($"Самый дешевый предмет: {worstItem} — {minCost}");

        Console.WriteLine("Товары дороже 100: ");
        foreach (KeyValuePair<string, int> item in dict)
        {
            if (item.Value > 100)
            {
                Console.Write($"{item.Key}: {item.Value} ");
            }
        }

        Console.WriteLine();
    }
}
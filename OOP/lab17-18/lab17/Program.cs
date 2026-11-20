class Program
{
    static void Main(string[] args)
    {
        Random random = new Random();
        List<int> numbers = new List<int>();
        for (int i = 0; i < 10; i++)
        {
            numbers.Add(random.Next(1, 100));
        }

        File.WriteAllLines("numbers.txt", numbers.Select(number => number.ToString()));

        List<int> loadedNumbers = new List<int>();
        foreach (string line in File.ReadAllLines("numbers.txt"))
        {
            loadedNumbers.Add(int.Parse(line));
        }

        int max = loadedNumbers[0];
        int min = loadedNumbers[0];
        int sum = 0;
        foreach (int number in loadedNumbers)
        {
            if (number > max)
            {
                max = number;
            }

            if (number < min)
            {
                min = number;
            }

            sum += number;
        }

        double average = (double)sum / loadedNumbers.Count;

        File.WriteAllLines("result.txt", new string[]
        {
            $"Максимум: {max}",
            $"Минимум: {min}",
            $"Среднее: {average:F2}"
        });

        Console.WriteLine($"Числа: {string.Join(", ", loadedNumbers)}");
        Console.WriteLine($"Максимум: {max}, минимум: {min}, среднее: {average:F2}");
        Console.WriteLine("Результат записан в файл result.txt");

        Directory.CreateDirectory("Source");
        File.WriteAllText(Path.Combine("Source", "file1.txt"), "Первый файл");
        File.WriteAllText(Path.Combine("Source", "file2.txt"), "Второй файл");

        Directory.CreateDirectory("Target");
        foreach (string file in Directory.GetFiles("Source", "*.txt"))
        {
            string targetPath = Path.Combine("Target", Path.GetFileName(file));
            File.Copy(file, targetPath, true);
            Console.WriteLine($"Скопирован: {Path.GetFileName(file)}");
        }
    }
}

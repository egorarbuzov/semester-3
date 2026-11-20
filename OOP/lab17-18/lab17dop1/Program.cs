class Program
{
    static void Main(string[] args)
    {
        string sourceFile = "bigfile.bin";
        string partsDir = "Parts";
        string mergedFile = "merged.bin";
        int partSize = 300 * 1024;

        byte[] data = new byte[1024 * 1024];
        Random random = new Random();
        random.NextBytes(data);
        File.WriteAllBytes(sourceFile, data);
        Console.WriteLine($"Создан файл {sourceFile}: {data.Length} байт");

        Directory.CreateDirectory(partsDir);
        int partsCount = SplitFile(sourceFile, partsDir, partSize);
        Console.WriteLine($"Файл разделен, частей: {partsCount}");

        string[] parts = Directory.GetFiles(partsDir);
        Array.Sort(parts);
        foreach (string part in parts)
        {
            Console.WriteLine($"  {Path.GetFileName(part)} — {new FileInfo(part).Length} байт");
        }

        MergeFiles(partsDir, mergedFile);
        Console.WriteLine($"Части объединены в {mergedFile}: {new FileInfo(mergedFile).Length} байт");

        byte[] mergedData = File.ReadAllBytes(mergedFile);
        Console.WriteLine($"Объединенный файл идентичен исходному: {data.SequenceEqual(mergedData)}");
    }

    static int SplitFile(string fileName, string partsDir, int partSize)
    {
        int partNumber = 1;

        using (FileStream source = new FileStream(fileName, FileMode.Open, FileAccess.Read))
        {
            byte[] buffer = new byte[partSize];
            while (true)
            {
                int bytesRead = source.Read(buffer, 0, buffer.Length);
                if (bytesRead == 0)
                {
                    break;
                }

                string partName = Path.Combine(partsDir, $"part{partNumber:D2}.bin");
                using (FileStream part = new FileStream(partName, FileMode.Create))
                {
                    part.Write(buffer, 0, bytesRead);
                }

                partNumber++;
            }
        }

        return partNumber - 1;
    }

    static void MergeFiles(string partsDir, string mergedFile)
    {
        string[] parts = Directory.GetFiles(partsDir);
        Array.Sort(parts);

        using (FileStream merged = new FileStream(mergedFile, FileMode.Create))
        {
            foreach (string part in parts)
            {
                using (FileStream source = new FileStream(part, FileMode.Open, FileAccess.Read))
                {
                    source.CopyTo(merged);
                }
            }
        }
    }
}

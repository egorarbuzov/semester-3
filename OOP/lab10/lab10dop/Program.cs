using System;

public enum SortAlgorithm
{
    Bubble,
    Selection,
    Insertion,
    Quick
}

public class Sorter<T> where T : IComparable<T>
{
    public int Comparisons { get; private set; }
    public int Swaps { get; private set; }

    public void Sort(T[] array, SortAlgorithm algorithm)
    {
        Comparisons = 0;
        Swaps = 0;

        if (array == null || array.Length < 2)
        {
            Console.WriteLine("Массив пуст или содержит 1 элемент — сортировка не требуется.");
            return;
        }

        switch (algorithm)
        {
            case SortAlgorithm.Bubble: BubbleSort(array); break;
            case SortAlgorithm.Selection: SelectionSort(array); break;
            case SortAlgorithm.Insertion: InsertionSort(array); break;
            case SortAlgorithm.Quick: QuickSort(array, 0, array.Length - 1); break;
        }

        Console.WriteLine($"Алгоритм: {algorithm}");
        Console.WriteLine($"Сравнений: {Comparisons}, Обменов: {Swaps}");
    }

    private void Swap(T[] a, int i, int j)
    {
        T temp = a[i];
        a[i] = a[j];
        a[j] = temp;
        Swaps++;
    }

    private void BubbleSort(T[] a)
    {
        for (int i = 0; i < a.Length - 1; i++)
        {
            bool swapped = false;
            for (int j = 0; j < a.Length - 1 - i; j++)
            {
                Comparisons++;
                if (a[j].CompareTo(a[j + 1]) > 0)
                {
                    Swap(a, j, j + 1);
                    swapped = true;
                }
            }
            if (!swapped) break;
        }
    }

    private void SelectionSort(T[] a)
    {
        for (int i = 0; i < a.Length - 1; i++)
        {
            int min = i;
            for (int j = i + 1; j < a.Length; j++)
            {
                Comparisons++;
                if (a[j].CompareTo(a[min]) < 0)
                    min = j;
            }
            if (min != i)
                Swap(a, i, min);
        }
    }

    private void InsertionSort(T[] a)
    {
        for (int i = 1; i < a.Length; i++)
        {
            T key = a[i];
            int j = i - 1;
            while (j >= 0)
            {
                Comparisons++;
                if (a[j].CompareTo(key) > 0)
                {
                    a[j + 1] = a[j];
                    Swaps++;
                    j--;
                }
                else break;
            }
            a[j + 1] = key;
            if (j + 1 != i) Swaps++;
        }
    }

    private void QuickSort(T[] a, int low, int high)
    {
        if (low < high)
        {
            int pivotIndex = Partition(a, low, high);
            QuickSort(a, low, pivotIndex - 1);
            QuickSort(a, pivotIndex + 1, high);
        }
    }

    private int Partition(T[] a, int low, int high)
    {
        T pivot = a[high];
        int i = low - 1;

        for (int j = low; j < high; j++)
        {
            Comparisons++;
            if (a[j].CompareTo(pivot) <= 0)
            {
                i++;
                if (i != j) Swap(a, i, j);
            }
        }
        if (i + 1 != high) Swap(a, i + 1, high);
        return i + 1;
    }
}

class Program
{
    static void Main()
    {
        Sorter<int> sorter = new Sorter<int>();

        int[] original = { 5, 2, 9, 1, 7, 3 };

        foreach (SortAlgorithm alg in Enum.GetValues(typeof(SortAlgorithm)))
        {
            int[] copy = (int[])original.Clone();
            Console.WriteLine($"\nИсходный массив: [{string.Join(", ", copy)}]");
            sorter.Sort(copy, alg);
            Console.WriteLine($"Результат: [{string.Join(", ", copy)}]");
        }
        
    }
}
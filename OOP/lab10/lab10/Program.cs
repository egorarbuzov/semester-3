using System;
using System.Data;

public interface IStorage<T> where T : struct
{
    void Add(T item);
    void Remove(int index);
    T Get(int index);
    int GetCount();
}

public class ArrayStorage<T> : IStorage<T> where T : struct
{
    private T[] storage;
    private int count;
    
    public  ArrayStorage()
    {
        storage = new T[100];
    }

    public void Add(T item)
    {
        try
        {
            if (count >= storage.Length)
            {
                throw new InvalidOperationException("Массив полон!!!");
            }
            storage[count] = item;
            count++;
            Console.WriteLine($"Элемент {item} добавлен.");
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Ошибка при добавлении: {ex.Message}");
        }
        finally
        {
            Console.WriteLine();
            Console.WriteLine("Метод Add завершен.");
        } 
    }

    public void Remove(int index)
    {
        try
        {
            if (count == 0)
            {
                throw new InvalidOperationException("Массив пуст!!!");
            }

            if (index >= 0 && index < storage.Length)
            {
                throw new InvalidOperationException("Выход за пределы массива!!!");
            }

            for (int i = index; i < index - 1; i++)
            {
                storage[i] = storage[i + 1];
            }

            count--;
            storage[count] = default(T);
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Ошибка: {ex.Message}");
        }
        finally
        {
            Console.WriteLine("Метод Remove выполнен");
        }
    }
    
    public T Get(int index)
    {
        try
        {
            if (index < 0 || index >= count)
            {
                throw new IndexOutOfRangeException($"Индекс {index} выходит за границы массива (0 - {count - 1}).");
            }
            return storage[index];
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Ошибка при получении: {ex.Message}");
            return default(T);
        }
        finally
        {
            Console.WriteLine("Метод Get завершен.");
        }
    }
    public int GetCount()
    {
        return count;
    }

}

class Program
{
    static void Main(string[] args)
    {
        Console.WriteLine("int");
        IStorage<int> intStorage = new ArrayStorage<int>();
        intStorage.Add(10);
        intStorage.Add(20);
        Console.WriteLine($"Get(0): {intStorage.Get(0)}");
        intStorage.Remove(0);
        Console.WriteLine($"Get(0) после Remove: {intStorage.Get(0)}");
        intStorage.Get(5);

        Console.WriteLine("\ndouble");
        IStorage<double> doubleStorage = new ArrayStorage<double>();
        doubleStorage.Add(3.14);
        doubleStorage.Add(2.71);
        Console.WriteLine($"Get(1): {doubleStorage.Get(1)}");
        doubleStorage.Remove(1);
        Console.WriteLine($"Get(1) после Remove: {doubleStorage.Get(1)}");

        Console.WriteLine("\nbool");
        IStorage<bool> boolStorage = new ArrayStorage<bool>();
        boolStorage.Add(true);
        Console.WriteLine($"Get(0): {boolStorage.Get(0)}");
        boolStorage.Remove(0);
        boolStorage.Remove(0);
        
    }
}
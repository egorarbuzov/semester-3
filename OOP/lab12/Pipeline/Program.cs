using System;
using System.Collections.Generic;

public class Pipeline<T>
{
    private List<Func<T, T>> _steps = new List<Func<T, T>>();

    public Pipeline<T> AddStep(Func<T, T> step)
    {
        _steps.Add(step);
        return this;
    }

    public Pipeline<T> InsertStep(int index, Func<T, T> step)
    {
        _steps.Insert(index, step);
        return this;
    }

    public Pipeline<T> RemoveStep(int index)
    {
        _steps.RemoveAt(index);
        return this;
    }

    public int GetStepCount()
    {
        return _steps.Count;
    }

    public void Clear()
    {
        _steps.Clear();
    }

    public T Execute(T input)
    {
        T current = input;
        foreach (Func<T, T> step in _steps)
            current = step(current);
        return current;
    }

    public T[] ExecuteAll(T[] inputs)
    {
        return Array.ConvertAll(inputs, x => Execute(x));
    }
}

class Program
{
    static void Main()
    {
        Pipeline<int> numberPipeline = new Pipeline<int>()
            .AddStep(x => x * 2)
            .AddStep(x => x + 10)
            .AddStep(x => x * x);

        Console.WriteLine($"Execute(5) = {numberPipeline.Execute(5)}");

        int[] numbers = { 1, 2, 3, 4, 5 };
        int[] processed = numberPipeline.ExecuteAll(numbers);
        Console.WriteLine("ExecuteAll = [" + string.Join(", ", processed) + "]");

        numberPipeline.InsertStep(1, x => x - 1);
        Console.WriteLine($"После InsertStep: Execute(5) = {numberPipeline.Execute(5)}");

        numberPipeline.RemoveStep(0);
        Console.WriteLine($"После RemoveStep: Execute(5) = {numberPipeline.Execute(5)}");

        numberPipeline.Clear();
        Console.WriteLine($"После Clear: шагов = {numberPipeline.GetStepCount()}, Execute(5) = {numberPipeline.Execute(5)}");

        Pipeline<string> textPipeline = new Pipeline<string>()
            .AddStep(s => s.Trim())
            .AddStep(s => s.ToLower())
            .AddStep(s => s.Replace(" ", "_"))
            .AddStep(s => $"[{s}]");

        Console.WriteLine($"Execute = {textPipeline.Execute("  Hello World  ")}");
    }
}
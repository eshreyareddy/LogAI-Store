"use client";
import { useEffect, useState } from "react";
import {
  LineChart, Line, XAxis, YAxis, CartesianGrid, Tooltip, ResponsiveContainer,
} from "recharts";

type MetricEntry = {
  timestamp: string;
  memtable_size: number;
};

export default function MemTableChart() {
  const [data, setData] = useState<MetricEntry[]>([]);

  useEffect(() => {
    const fetchMetrics = async () => {
      try {
        const res = await fetch("http://127.0.0.1:8000/metrics");
        const json = await res.json();
        setData(json);
      } catch (err) {
        console.error("Failed to fetch metrics:", err);
      }
    };

    fetchMetrics();
    const interval = setInterval(fetchMetrics, 3000);
    return () => clearInterval(interval);
  }, []);

  return (
    <div className="bg-white p-6 rounded-xl shadow-md mt-8">
      <h2 className="text-xl font-semibold mb-2">📈 MemTable Size Over Time</h2>
      <ResponsiveContainer width="100%" height={300}>
        <LineChart data={data}>
          <CartesianGrid strokeDasharray="3 3" />
          <XAxis dataKey="timestamp" tick={{ fontSize: 10 }} />
          <YAxis />
          <Tooltip />
          <Line type="monotone" dataKey="memtable_size" stroke="#6366F1" strokeWidth={2} />
        </LineChart>
      </ResponsiveContainer>
    </div>
  );
}

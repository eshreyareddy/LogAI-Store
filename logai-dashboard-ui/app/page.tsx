"use client";
import { useEffect, useState } from "react";
import toast, { Toaster } from "react-hot-toast";
import MemTableChart from "./components/MemTableChart";

type StatusData = {
  sstable_count: number;
  sstables: string[];
  wal_exists: boolean;
  memtable_size: number;
  tombstone_count: number;
  compaction_count: number;
  last_compaction_time: string;
};

export default function Home() {
  const [status, setStatus] = useState<StatusData | null>(null);
  const [key, setKey] = useState("");
  const [value, setValue] = useState("");
  const [deleteKey, setDeleteKey] = useState("");

  const fetchStatus = async () => {
    try {
      const res = await fetch("http://127.0.0.1:8000/status");
      const data = await res.json();
      setStatus(data);
    } catch (err) {
      console.error("Failed to fetch status:", err);
    }
  };

  useEffect(() => {
    fetchStatus();
    const interval = setInterval(fetchStatus, 3000);
    return () => clearInterval(interval);
  }, []);

  const handleInsert = async () => {
    if (!key || !value) return toast.error("Please enter key and value");
    try {
      await fetch("http://127.0.0.1:8000/put", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ key, value }),
      });
      setKey("");
      setValue("");
      toast.success("✅ Key inserted!");
      fetchStatus();
    } catch (err) {
      console.error("Insert failed:", err);
      toast.error("❌ Insert failed");
    }
  };

  const handleDelete = async () => {
    if (!deleteKey) return toast.error("Please enter a key to delete");
    try {
      await fetch("http://127.0.0.1:8000/delete", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ key: deleteKey }),
      });
      setDeleteKey("");
      toast.success("🗑️ Key deleted!");
      fetchStatus();
    } catch (err) {
      console.error("Delete failed:", err);
      toast.error("❌ Delete failed");
    }
  };

  return (
    <main className="min-h-screen bg-gray-100 p-10 text-gray-900">
      <Toaster position="top-right" />
      <h1 className="text-3xl font-bold mb-6">📊 LogAI-Store Dashboard</h1>

      {/* 🛠️ Controls */}
      <div className="bg-white p-6 rounded-xl shadow-md mb-6">
        <h2 className="text-xl font-bold mb-4">🛠️ Controls</h2>

        <div className="mb-4 flex flex-wrap gap-2">
          <input
            type="text"
            placeholder="Key"
            value={key}
            onChange={(e) => setKey(e.target.value)}
            className="border p-2 rounded flex-grow"
          />
          <input
            type="text"
            placeholder="Value"
            value={value}
            onChange={(e) => setValue(e.target.value)}
            className="border p-2 rounded flex-grow"
          />
          <button
            onClick={handleInsert}
            className="bg-green-600 text-white px-4 py-2 rounded hover:bg-green-700"
          >
            Insert
          </button>
        </div>

        <div className="flex flex-wrap gap-2">
          <input
            type="text"
            placeholder="Key to delete"
            value={deleteKey}
            onChange={(e) => setDeleteKey(e.target.value)}
            className="border p-2 rounded flex-grow"
          />
          <button
            onClick={handleDelete}
            className="bg-red-600 text-white px-4 py-2 rounded hover:bg-red-700"
          >
            Delete
          </button>
        </div>
      </div>

      {!status ? (
        <p className="text-gray-600">Loading...</p>
      ) : (
        <div className="space-y-6">
          <div className="bg-white p-6 rounded-xl shadow-md">
            <h2 className="text-xl font-semibold">📦 SSTable Count</h2>
            <p className="text-2xl text-blue-600">{status.sstable_count}</p>
          </div>

          <div className="bg-white p-6 rounded-xl shadow-md">
            <h2 className="text-xl font-semibold">📁 SSTable Files</h2>
            <ul className="list-disc list-inside text-gray-700">
              {status.sstables.map((file, i) => (
                <li key={i}>{file}</li>
              ))}
            </ul>
          </div>

          <div className="bg-white p-6 rounded-xl shadow-md">
            <h2 className="text-xl font-semibold">📝 WAL Status</h2>
            <p className={`text-lg ${status.wal_exists ? "text-green-600" : "text-red-600"}`}>
              {status.wal_exists ? "Present ✅" : "Missing ❌"}
            </p>
          </div>

          <div className="bg-white p-6 rounded-xl shadow-md">
            <h2 className="text-xl font-semibold">🧠 MemTable Size</h2>
            <p className="text-lg text-indigo-600">
              {status.memtable_size !== undefined ? `${status.memtable_size} keys` : "Loading..."}
            </p>
          </div>

          <div className="bg-white p-6 rounded-xl shadow-md">
            <h2 className="text-xl font-semibold">🗑 Tombstone Count</h2>
            <p className="text-lg text-rose-600">
              {status.tombstone_count !== undefined ? status.tombstone_count : "Loading..."}
            </p>
          </div>

          <div className="bg-white p-6 rounded-xl shadow-md">
            <h2 className="text-xl font-semibold">🔄 Compaction Count</h2>
            <p className="text-lg text-blue-600">{status.compaction_count}</p>
          </div>

          <div className="bg-white p-6 rounded-xl shadow-md">
            <h2 className="text-xl font-semibold">🕒 Last Compaction Time</h2>
            <p className="text-lg text-gray-700">{status.last_compaction_time}</p>
          </div>

          {/* 🟦 Live Chart */}
          <MemTableChart />
        </div>
      )}
    </main>
  );
}

# 📦 LogAI-Store

**LogAI-Store** is a machine learning–driven hybrid storage engine that combines the strengths of **LSM Trees** and **B-Trees**, designed for intelligent, real-time data ingestion and adaptive query optimization. Powered by predictive caching and workload-aware compaction strategies, LogAI-Store bridges the gap between high-throughput writes and low-latency reads.

---

## 🚀 Features

- ✅ Real-time data ingestion and live monitoring
- 🌲 Hybrid **LSM Tree + B-Tree** architecture
- 🔄 Advanced compaction & write optimization strategies
- 🧠 Machine Learning–based predictive caching
- 📈 Real-time visualization of metrics using **Recharts** & **D3.js**
- 🧪 Insert/Delete operations powered via `commands.txt`

---

## 🧠 Architecture

The project is split into two major components:

| Component            | Description                                                  |
|----------------------|--------------------------------------------------------------|
| `logai-dashboard`    | Backend engine built with **FastAPI**, managing ingestion and compaction |
| `logai-dashboard-ui` | Real-time monitoring dashboard built with **Next.js + React** |

---

## 🗂️ Directory Structure

```
logai-store/
├── logai-dashboard/          # Backend engine (FastAPI)
│   ├── main.py               # Entry point
│   ├── commands.txt          # File with DB commands (insert/delete)
│   └── venv/                 # Python virtual environment
├── logai-dashboard-ui/       # Frontend UI (Next.js)
│   ├── app/                  # App logic & state
│   ├── pages/                # Route components
│   ├── public/               # Static assets
│   └── node_modules/         # Dependencies
└── Test/                     # Unit & integration tests
```

---

## ⚙️ Setup Instructions

### 1️⃣ Clone the Repository

```bash
git clone https://github.com/eshreyareddy/logai-store.git
cd logai-store
```

---

### 2️⃣ Backend Setup (`logai-dashboard`)

```bash
cd logai-dashboard

# Create & activate virtual environment
python3 -m venv venv

# Activate (Linux/macOS):
source venv/bin/activate

# Activate (Windows):
.\venv\Scripts\activate

# Install dependencies
pip install -r requirements.txt

# Start backend server
python main.py
```

---

### 3️⃣ Frontend Setup (`logai-dashboard-ui`)

```bash
cd ../logai-dashboard-ui

# Install frontend dependencies
npm install

# Start development server
npm run dev
```

---

## 💻 Usage

- 🖥️ UI Dashboard → http://localhost:3000  
- 🧠 Backend API → http://localhost:8000  
- ⚙️ Interact using the `commands.txt` file to simulate insert/delete/query operations

---

## 🧰 Tech Stack

| Layer         | Technology                                   |
|---------------|----------------------------------------------|
| **Backend**   | Python, FastAPI                              |
| **Frontend**  | React, Next.js, Tailwind CSS                 |
| **Database**  | Hybrid LSM Trees & B-Trees                   |
| **ML Models** | Scikit-learn, TensorFlow                     |
| **Charts**    | Recharts, D3.js                              |
| **Infra**     | Localhost (dev), Docker/K8s-ready (future)  |

---

## 🤝 Contributing

Contributions are welcome and appreciated!

### 📌 To contribute:

```bash
# Step 1: Fork this repository

# Step 2: Create your branch
git checkout -b feature/your-feature

# Step 3: Commit your changes
git commit -m "Add feature: your-feature"

# Step 4: Push your branch
git push origin feature/your-feature

# Step 5: Open a Pull Request 🚀
```

Please follow the existing file structure and write clean, documented code.

---

## 📝 License

This project is licensed under the **MIT License** — see the [LICENSE](LICENSE) file for details.


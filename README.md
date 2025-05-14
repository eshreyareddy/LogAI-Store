LogAI-Store

Project Overview

LogAI-Store is an advanced ML-driven hybrid storage engine combining B-Trees and LSM Trees with adaptive tuning based on workload analysis. It features real-time data ingestion, compaction, and intelligent data retrieval powered by machine learning. The project is structured into two main components:

logai-dashboard: A monitoring dashboard built with Next.js for real-time visualization of data ingestion, compaction stats, and performance metrics.

logai-dashboard-ui: The frontend UI layer, designed with React and Next.js, providing intuitive interfaces for interacting with the storage engine.

Features

Real-time data ingestion and monitoring

Hybrid LSM Tree and B-Tree architecture

Advanced compaction strategies

Predictive caching with machine learning

Real-time visualization of storage and query performance

Directory Structure

logai-store/
├── logai-dashboard          # Backend engine with FastAPI
│   ├── main.py             # Main entry point for FastAPI
│   ├── commands.txt        # Command inputs for database operations
│   └── venv/               # Virtual environment (not pushed to GitHub)
│
├── logai-dashboard-ui      # Frontend dashboard with Next.js
│   ├── app/                # Main application components
│   ├── node_modules/       # Dependencies
│   ├── public/             # Static assets
│   └── pages/              # Next.js page components
│
└── Test                    # Testing scripts and unit tests

Setup Instructions

Clone the repository:

git clone https://github.com/eshreyareddy/logai-store.git
cd logai-store

Backend Setup (logai-dashboard):

cd logai-dashboard
python3 -m venv venv
source venv/bin/activate   # For Linux/Mac
.\venv\Scripts\activate  # For Windows
pip install -r requirements.txt
python main.py

Frontend Setup (logai-dashboard-ui):

cd ../logai-dashboard-ui
npm install
npm run dev

Usage

Access the UI at http://localhost:3000

Backend runs on http://localhost:8000

Use commands.txt for testing database operations

Technologies Used

Backend: FastAPI, Python

Frontend: React, Next.js

Database: LSM Trees, B-Trees

Visualization: Recharts, D3.js

Machine Learning: Scikit-Learn, TensorFlow

Contributing

Contributions are welcome! Please follow the guidelines:

Fork the repository

Create a new branch (feature/your-feature)

Commit your changes (git commit -m 'Add some feature')

Push to the branch (git push origin feature/your-feature)

Open a Pull Request

License

This project is licensed under the MIT License - see the LICENSE file for details.
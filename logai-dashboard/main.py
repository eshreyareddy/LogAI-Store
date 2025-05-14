from fastapi import FastAPI, Request
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel
import os
import json

app = FastAPI()

# CORS setup to allow Next.js frontend
app.add_middleware(
    CORSMiddleware,
    allow_origins=["http://localhost:3000"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

# 📄 Pydantic models for /put and /delete
class PutRequest(BaseModel):
    key: str
    value: str

class DeleteRequest(BaseModel):
    key: str

# ✅ GET /status
@app.get("/status")
def get_status():
    data_folder = "C:/Users/S_Edulakanti/Desktop/Test/data"
    files = os.listdir(data_folder)

    sstables = [f for f in files if f.startswith("sstable") and f.endswith(".txt")]
    wal_exists = "wal.log" in files

    # Read status.json for metrics
    status_file = os.path.join(data_folder, "status.json")
    memtable_size = 0
    tombstone_count = 0
    compaction_count = 0
    last_compaction_time = "N/A"

    if os.path.exists(status_file):
        with open(status_file, "r") as f:
            data = json.load(f)
            memtable_size = data.get("memtable_size", 0)
            tombstone_count = data.get("tombstone_count", 0)
            compaction_count = data.get("compaction_count", 0)
            last_compaction_time = data.get("last_compaction_time", "N/A")

    return {
        "sstable_count": len(sstables),
        "sstables": sstables,
        "wal_exists": wal_exists,
        "memtable_size": memtable_size,
        "tombstone_count": tombstone_count,
        "compaction_count": compaction_count,
        "last_compaction_time": last_compaction_time
    }

# ✅ POST /put
@app.post("/put")
async def put_key(body: PutRequest):
    key = body.key
    value = body.value

    # Save the insert command
    with open("commands.txt", "w") as f:
        f.write(f"put {key} {value}")

    # Trigger C++ backend
    os.system("C:\\Users\\S_Edulakanti\\Desktop\\Test\\logai-store.exe")
    return {"status": "ok", "message": f"Inserted {key} = {value}"}

# ✅ POST /delete
@app.post("/delete")
async def delete_key(body: DeleteRequest):
    key = body.key

    # Save the delete command
    with open("commands.txt", "w") as f:
        f.write(f"delete {key}")

    # Trigger C++ backend
    os.system("C:\\Users\\S_Edulakanti\\Desktop\\Test\\logai-store.exe")
    return {"status": "ok", "message": f"Deleted key {key}"}

from fastapi.responses import JSONResponse

@app.get("/metrics")
def get_metrics():
    metrics_path = "C:/Users/S_Edulakanti/Desktop/Test/data/metrics.json"
    if not os.path.exists(metrics_path):
        return JSONResponse(content={"error": "metrics.json not found"}, status_code=404)

    with open(metrics_path, "r") as f:
        data = json.load(f)
        return data

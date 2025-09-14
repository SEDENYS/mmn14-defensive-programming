import subprocess
import time
from concurrent.futures import ThreadPoolExecutor

CLIENTS = 5  # 🔢 כמה לקוחות במקביל (שנה ל-10 אם תרצה יותר)

def run_client(i):
    print(f"🚀 Starting client {i}")
    # אפשר להריץ full_test.py או mmn14client.py
    result = subprocess.run(["python3", "full_test.py"], capture_output=True, text=True)
    print(f"✅ Client {i} finished\n--- Output ---\n{result.stdout}\n")

def main():
    start = time.time()
    with ThreadPoolExecutor(max_workers=CLIENTS) as executor:
        futures = [executor.submit(run_client, i) for i in range(1, CLIENTS + 1)]
        for f in futures:
            f.result()
    end = time.time()
    print(f"⏱️ Total test time: {end - start:.2f} seconds")

if __name__ == "__main__":
    main()

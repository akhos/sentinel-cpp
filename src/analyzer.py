import json

def analyze_scan(filepath):
    try:
        with open(filepath, 'r') as f:
            data = json.load(f)
            
        print(f"--- Sentinel Python Analysis: {data['filename']} ---")
        print(f"File Size: {data['file_size']} bytes")
        print(f"Global Entropy: {data['total_entropy']:.4f}")
        
        # Calculate some stats on the windows
        entropies = data['window_entropies']
        if not entropies:
            print("No window data found.")
            return

        peak = max(entropies)
        avg = sum(entropies) / len(entropies)
        
        print(f"Window Average: {avg:.4f}")
        print(f"Window Peak:    {peak:.4f}")

        # Basic Heuristic: If peak is much higher than average, 
        # it's a sign of a hidden payload.
        if peak > 7.5:
            print("\n[!] VERDICT: HIGH RISK")
            print(f"    Reason: Found localized entropy spike of {peak}")
        elif avg > 6.0:
            print("\n[!] VERDICT: MEDIUM RISK")
            print("    Reason: High overall entropy (likely a compressed binary)")
        else:
            print("\n[+] VERDICT: LOW RISK")
            
    except FileNotFoundError:
        print("Error: scan_results.json not found. Did you run the C++ program?")

if __name__ == "__main__":
    analyze_scan('scan_results.json')
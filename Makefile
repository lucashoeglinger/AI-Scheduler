CXX          := g++
CXXFLAGS     := -std=c++17 -Wall -Wextra -Wpedantic -Wshadow \
                -Wconversion -Wsign-conversion -O2 -Iinclude

SRC_DIR      := src
BUILD_DIR    := build
TARGET       := queue-wrapper
INSTALL_PATH := /usr/local/bin

PACKAGE_NAME := gpu_queue_wrapper
VERSION      := 1.0.0
DIST_DIR     := $(PACKAGE_NAME)-$(VERSION)
TEST_DB      := /tmp/test_gpu_queue.txt

SRCS         := $(wildcard $(SRC_DIR)/*.cpp)
OBJS         := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

all: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "[BUILD] Erfolgreich gebaut: $@"

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(DIST_DIR) $(DIST_DIR).tar.gz
	@echo "[CLEAN] Build- und Dist-Ordner gelöscht."

install: all
	@echo "[INSTALL] Kopiere $(TARGET) nach $(INSTALL_PATH)..."
	sudo install -m 755 $(BUILD_DIR)/$(TARGET) $(INSTALL_PATH)/$(TARGET)

check: all
	@echo "[CHECK] Starte Selbsttests..."
	@touch $(TEST_DB) && chmod 666 $(TEST_DB)
	@test -f $(BUILD_DIR)/$(TARGET) || (echo "[CHECK FAIL] Binary fehlt!"; exit 1)
	@test -x $(BUILD_DIR)/$(TARGET) || (echo "[CHECK FAIL] Binary ist nicht ausführbar!"; exit 1)
	@rm -f $(TEST_DB)
	@echo "[CHECK PASS] Alle Überprüfungen erfolgreich bestanden!"

dist: clean
	@echo "[DIST] Erstelle Release-Tarball $(DIST_DIR).tar.gz..."
	@mkdir -p $(DIST_DIR)
	@cp -r include src Makefile README.md $(DIST_DIR)/ 2>/dev/null || cp -r include src Makefile $(DIST_DIR)/
	@tar -czf $(DIST_DIR).tar.gz $(DIST_DIR)
	@rm -rf $(DIST_DIR)
	@echo "[DIST] Release-Archiv $(DIST_DIR).tar.gz erstellt."

distcheck: dist
	@echo "[DISTCHECK] Prüfe Distribution im isolierten Build..."
	@tar -xzf $(DIST_DIR).tar.gz
	@cd $(DIST_DIR) && $(MAKE) all
	@cd $(DIST_DIR) && $(MAKE) check
	@cd $(DIST_DIR) && $(MAKE) clean
	@rm -rf $(DIST_DIR) $(DIST_DIR).tar.gz
	@echo "[DISTCHECK PASS] Die Distribution ist vollständig und baut fehlerfrei!"

.PHONY: all clean install check dist distcheck
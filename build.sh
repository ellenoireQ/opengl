#!/bin/bash

# Colors for output
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m' # No Color

echo -e "${BLUE}=== OpenGL Chapter Builder ===${NC}\n"

# Find all gs_* folders
chapters=($(find . -maxdepth 1 -type d -name "gs_*" | sort))

if [ ${#chapters[@]} -eq 0 ]; then
  echo -e "${RED}No chapters found (gs_* folders)${NC}"
  exit 1
fi

# Display chapter list
echo -e "${YELLOW}Select chapter to build:${NC}"
for i in "${!chapters[@]}"; do
  chapter_name=$(basename "${chapters[$i]}")
  echo "  $((i + 1)). $chapter_name"
done
echo "  0. Build all chapters"
echo ""

# Input selection
read -p "Enter choice (0-${#chapters[@]}): " choice

# Validate input
if ! [[ "$choice" =~ ^[0-9]+$ ]] || [ "$choice" -gt "${#chapters[@]}" ]; then
  echo -e "${RED}Invalid choice!${NC}"
  exit 1
fi

# Function to build one chapter
build_chapter() {
  local chapter_dir=$1
  local chapter_name=$(basename "$chapter_dir")
  local should_run=$2

  echo -e "\n${GREEN}Building $chapter_name...${NC}"

  # Find .cpp file in chapter folder
  cpp_file=$(find "$chapter_dir" -maxdepth 1 -name "*.cpp" | head -n 1)

  if [ -z "$cpp_file" ]; then
    echo -e "${RED}No .cpp file found in $chapter_name${NC}"
    return 1
  fi

  # Create build directory if it doesn't exist
  mkdir -p build
  cd build

  # Generate CMake with selected target file
  cmake .. -DTARGET="$cpp_file" >/dev/null 2>&1

  if [ $? -ne 0 ]; then
    echo -e "${RED}CMake configuration failed for $chapter_name${NC}"
    cd ..
    return 1
  fi

  # Build
  cmake --build . >/dev/null 2>&1

  if [ $? -eq 0 ]; then
    echo -e "${GREEN}✓ $chapter_name built successfully${NC}"
    # Rename executable
    mv my_app "${chapter_name}" 2>/dev/null
    
    # Run if requested
    if [ "$should_run" = "true" ]; then
      echo -e "${BLUE}Running $chapter_name...${NC}\n"
      ./"${chapter_name}"
      local exit_code=$?
      echo -e "\n${BLUE}Program exited with code: $exit_code${NC}"
    fi
    
    cd ..
    return 0
  else
    echo -e "${RED}✗ Build failed for $chapter_name${NC}"
    cd ..
    return 1
  fi
}

# Build based on selection
if [ "$choice" -eq 0 ]; then
  echo -e "\n${BLUE}Building all chapters...${NC}"
  success=0
  failed=0

  for chapter in "${chapters[@]}"; do
    if build_chapter "$chapter" "false"; then
      ((success++))
    else
      ((failed++))
    fi
  done

  echo -e "\n${BLUE}=== Build Results ===${NC}"
  echo -e "${GREEN}Success: $success${NC}"
  echo -e "${RED}Failed: $failed${NC}"
  echo -e "\n${BLUE}Executables are in build/ folder${NC}"
else
  selected_chapter="${chapters[$((choice - 1))]}"
  build_chapter "$selected_chapter" "true"
fi

export function camelCaseToTitleCase(input: string): string {
  return (
    input
      // Add space before capital letters
      .replace(/([a-z])([A-Z])/g, "$1 $2")
      // Capitalize first letter of each word
      .replace(/\b\w/g, (char) => char.toUpperCase())
  );
}

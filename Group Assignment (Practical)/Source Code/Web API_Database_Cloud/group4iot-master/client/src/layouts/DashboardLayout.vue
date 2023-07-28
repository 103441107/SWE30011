<template>
  <div class="flex h-screen bg-gray-200 font-roboto">
    <div class="flex-1 flex flex-col overflow-hidden">
      <main class="flex-1 overflow-x-hidden overflow-y-auto bg-gray-200">
        <div class="container mx-auto px-6 py-8 max-h-full h-full">
          <slot />
        </div>
      </main>
    </div>
  </div>
</template>
